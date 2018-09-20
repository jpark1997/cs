#include <errno.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "myMalloc.h"
#include "printing.h"

/* Due to the way assert() prints error messges we use out own assert function
 * for deteminism when testing assertions
 */
#ifdef TEST_ASSERT
  inline static void assert(int e) {
    if (!e) {
      const char * msg = "Assertion Failed!\n";
      write(2, msg, strlen(msg));
      exit(1);
    }
  }
#else
  #include <assert.h>
#endif

/*
 * Mutex to ensure thread safety for the freelist
 */
static pthread_mutex_t mutex;

/*
 * Array of sentinel nodes for the freelists
 */
header freelistSentinels[N_LISTS];

/*
 * Pointer to the second fencepost in the most recently allocated chunk from
 * the OS. Used for coalescing chunks
 */
header * lastFencePost;

/*
 * Pointer to maintian the base of the heap to allow printing based on the
 * distance from the base of the heap
 */ 
void * base;

/*
 * List of chunks allocated by  the OS for printing boundary tags
 */
header * osChunkList [MAX_OS_CHUNKS];
size_t numOsChunks = 0;

/*
 * direct the compiler to run the init function before running main
 * this allows initialization of required globals
 */
static void init (void) __attribute__ ((constructor));

// Helper functions for manipulating pointers to headers
static inline header * get_header_from_offset(void * ptr, ptrdiff_t off);
static inline header * get_left_header(header * h);
static inline header * ptr_to_header(void * p);

// Helper functions for allocating more memory from the OS
static inline void initialize_fencepost(header * fp, size_t left_size);
static inline void insert_os_chunk(header * hdr);
static inline void insert_fenceposts(void * raw_mem, size_t size);
static header * allocate_chunk(size_t size);

// Helper functions for freeing a block
static inline void deallocate_object(void * p);

// Helper functions for allocating a block
static inline header * allocate_object(size_t raw_size);
size_t roundUp(size_t size);
bool helpAllocateChunk();


// Helper functions for verifying that the data structures are structurally 
// valid
static inline header * detect_cycles();
static inline header * verify_pointers();
static inline bool verify_freelist();
static inline header * verify_chunk(header * chunk);
static inline bool verify_tags();

static void init();

static bool isMallocInitialized;

/**
 * @brief Helper function to retrieve a header pointer from a pointer and an 
 *        offset
 *
 * @param ptr base pointer
 * @param off number of bytes from base pointer where header is located
 *
 * @return a pointer to a header offset bytes from pointer
 */
static inline header * get_header_from_offset(void * ptr, ptrdiff_t off) {
	return (header *)((char *) ptr + off);
}

/**
 * @brief Helper function to get the header to the right of a given header
 *
 * @param h original header
 *
 * @return header to the right of h
 */
header * get_right_header(header * h) {
	return get_header_from_offset(h, get_size(h));
}

/**
 * @brief Helper function to get the header to the left of a given header
 *
 * @param h original header
 *
 * @return header to the right of h
 */
inline static header * get_left_header(header * h) {
  return get_header_from_offset(h, -h->left_size);
}

/**
 * @brief Fenceposts are marked as always allocated and may need to have
 * a left object size to ensure coalescing happens properly
 *
 * @param fp a pointer to the header being used as a fencepost
 * @param left_size the size of the object to the left of the fencepost
 */
inline static void initialize_fencepost(header * fp, size_t left_size) {
	set_state(fp,FENCEPOST);
	set_size(fp, ALLOC_HEADER_SIZE);
	fp->left_size = left_size;
}

/**
 * @brief Helper function to maintain list of chunks from the OS for debugging
 *
 * @param hdr the first fencepost in the chunk allocated by the OS
 */
inline static void insert_os_chunk(header * hdr) {
  if (numOsChunks < MAX_OS_CHUNKS) {
    osChunkList[numOsChunks++] = hdr;
  }
}

/**
 * @brief given a chunk of memory insert fenceposts at the left and 
 * right boundaries of the block to prevent coalescing outside of the
 * block
 *
 * @param raw_mem a void pointer to the memory chunk to initialize
 * @param size the size of the allocated chunk
 */
inline static void insert_fenceposts(void * raw_mem, size_t size) {
  // Convert to char * before performing operations
  char * mem = (char *) raw_mem;

  // Insert a fencepost at the left edge of the block
  header * leftFencePost = (header *) mem;
  initialize_fencepost(leftFencePost, ALLOC_HEADER_SIZE);

  // Insert a fencepost at the right edge of the block
  header * rightFencePost = get_header_from_offset(mem, size - ALLOC_HEADER_SIZE);
  initialize_fencepost(rightFencePost, size - 2 * ALLOC_HEADER_SIZE);
}

/**
 * @brief Allocate another chunk from the OS and prepare to insert it
 * into the free list
 *
 * @param size The size to allocate from the OS
 *
 * @return A pointer to the allocable block in the chunk (just after the 
 * first fencpost)
 */
static header * allocate_chunk(size_t size) {
  void * mem = sbrk(size);
  
  insert_fenceposts(mem, size);
  header * hdr = (header *) ((char *)mem + ALLOC_HEADER_SIZE);
  set_state(hdr, UNALLOCATED);
  set_size(hdr, size - 2 * ALLOC_HEADER_SIZE);
  hdr->left_size = ALLOC_HEADER_SIZE;
  return hdr;
}

size_t roundUp(size_t size) {
	size_t rDown = (size/8)*8;
	size_t rUp = rDown + 8;
	if(size - rDown == 0) {
		return size;
	} else {
		return rUp;
	}
}

/**
 * @brief Helper allocate an object given a raw request size from the user
 *
 * @param raw_size number of bytes the user needs
 *
 * @return A block satisfying the user's request
 */

bool helpAllocateChunk(){
  
//  if(flag == 2) {
	header * nh = allocate_chunk(ARENA_SIZE);
	if(nh == NULL) {
		errno = ENOMEM;
		return false;
	}
	header * nhLeft = get_left_header(nh);//leftFencepost of new chunk
	header * checkLFP = get_left_header(nhLeft);//left of nhLeft
	header * rightFp = get_right_header(nh);
	
//	printf("if(flag == 2)\n");
//	printf("lastfencepost = %p\n", lastFencePost);
//	printf("checkLFP = %p\n", checkLFP);
//	printf("nh = %p\n", nh);
//	printf("nhLeft = %p\n", nhLeft);
	if(checkLFP == lastFencePost) {//when it is adjacent
//		printf("301\n");
		header * lfpLeft = get_left_header(checkLFP);
//		printf("303\n");
		enum state isAlloc = get_state(lfpLeft);
	//	header * rightFp = get_right_header(nh);

//		printf("if(checkLFP\n");
		if(isAlloc) {
//			printf("allocated\n");
			size_t lfpSize = get_size(checkLFP);
			size_t fpSize = get_size(nhLeft);
			size_t nhSize = get_size(nh);
			size_t sum = lfpSize + fpSize + nhSize;

			set_size(checkLFP, sum);
			set_state(checkLFP, 0);

			rightFp->left_size = sum;
			set_size(nhLeft, 0);
			set_size(nh, 0);
			
			header * free11 = &freelistSentinels[N_LISTS - 1];
			header * block11 = free11->next;
			free11->next = checkLFP;
			checkLFP->next = block11;
			checkLFP->prev = &freelistSentinels[N_LISTS - 1];
			checkLFP->next->prev = checkLFP;
			checkLFP->prev->next = checkLFP;

			lastFencePost = rightFp;
		} else if (!isAlloc) {//when the header left to lfp is unallocated
//			printf("unallocated\n");
			size_t lfpSize = get_size(checkLFP);
			size_t fpSize = get_size(nhLeft);
			size_t nhSize = get_size(nh);
			size_t lfpLSize = get_size(lfpLeft);
			size_t sum = lfpSize + fpSize + nhSize + lfpLSize;

			set_size(lfpLeft, sum);
			set_state(lfpLeft, 0);

			rightFp->left_size = sum;
			set_size(nh, 0);
			set_size(nhLeft, 0);
			set_size(checkLFP, 0);

			size_t indexUA = ((lfpLSize - ALLOC_HEADER_SIZE)/8) - 1;
			if(indexUA > 58) {
				indexUA = 58;
			}
			if(indexUA != 58) {
			header * uf = &freelistSentinels[indexUA];
			header * ufb = uf->next;

			while(ufb != uf) {
				if(ufb == lfpLeft) {
					break;
				}
				ufb = ufb->next;
			}

			ufb->next->prev = ufb->prev;
			ufb->prev->next = ufb->next;

			header * rf = &freelistSentinels[N_LISTS - 1];
			header * rfb = rf->next;
			rf->next = lfpLeft;
			lfpLeft->next = rfb;
			lfpLeft->prev = &freelistSentinels[N_LISTS - 1];
			lfpLeft->next->prev = lfpLeft;
			lfpLeft->prev->next = lfpLeft;
			} else {
				lfpLeft->next->left_size = sum;
			}
			lastFencePost = rightFp;
		}
	} else {
//		printf("else373\n");
		header * rf = &freelistSentinels[N_LISTS - 1];
		header * rfb = rf->next;
		rf->next = nh;
		nh->next = rfb;
		nh->prev = &freelistSentinels[N_LISTS - 1];
		nh->next->prev = nh;
		nh->prev->next = nh;

		insert_os_chunk(nhLeft);
		lastFencePost = rightFp;
	}
 // }
	return true;

}
  //printf("383\n");
static inline header * allocate_object(size_t raw_size) {
	(void) raw_size;
	if(raw_size == 0) {
		return NULL;
	}

	size_t asize;
	size_t rounded = roundUp(raw_size);

	if(raw_size < 32) {
		asize = 32;
	} else {
		asize = rounded + ALLOC_HEADER_SIZE;
	}
	header * freelist;
	header * block;

	int i = 0;
//	int flag = 2;
	//size_t blsize;

//	while(flag != 2) {

	for(i = 0; i < N_LISTS; i++) {
		freelist = &freelistSentinels[i];
		block = freelist->next;

		size_t blocksize = get_size(block);
		if((blocksize != 0) && (blocksize >= asize)) {
			break;
		}
	}

	size_t blsize = get_size(block);

	blsize = (int) blsize;
	asize = (int) asize;

	int diff = blsize - asize;

	int flag = 0;
	if((diff < 32) && (diff >= 0)) {
		flag = 1;
	} else if (diff >= 32) {
		flag = 0;
	} else if (diff < 0) {
		flag = 2;
	}


////////////////
	while(flag == 2) {

		bool a = helpAllocateChunk();
		if(a == false) {
			return NULL;
		}

		for(i = 0; i < N_LISTS; i++) {
			freelist = &freelistSentinels[i];
			block = freelist->next;
			size_t blocksize = get_size(block);
			if((blocksize != 0) && (blocksize >= asize)) {
				break;
			}
		}

		blsize = get_size(block);
		blsize = (int) blsize;
		asize = (int) asize;

		int diff = blsize - asize;

		if((diff < 32) && (diff >= 0)) {
			flag = 1;
		} else if (diff >= 32) {
			flag = 0;
		} else if (diff < 0) {
			flag = 2;
		}

	}

//	}

//printf("409 flag = %d\n", flag);
  block = freelist->next;
  header * hdr;
 // while(block != freelist) {
//  	printf("while\n");
	if(flag == 1) {
//		printf("if\n");
		set_state(block, 1);
		//printf("if\n");	
		block->next->prev = block->prev;
		block->prev->next = block->next;

		lastFencePost = get_right_header(block);
//		return block;
		hdr = block;
//		break;
	} else if(flag == 0) {
//		printf("else if\n");

		header * right = get_right_header(block);
		right->left_size = asize;
		size_t newsize = blsize - asize;
		set_size(block, newsize);
		
//		printf("aaaa\n");
		//char * temp = (char *) block + get_size(block);

		//header * newH = (header *) temp;

		header * newH = get_left_header(right);
		//header *fp = get_header_from_offset(block, blsize);
		
//		printf("ccccc\n");
		set_size(newH, asize);
		//set_size(newH->prev, newsize);
		set_state(newH, 1);
		newH->left_size = newsize;
		
		//header * rfp = get_header_from_offset(newH, asize);
		//rfp->left_size = asize;

		size_t index = ((newsize - ALLOC_HEADER_SIZE)/8) - 1;
		index = (int) index;
		
		if(index > 58) {
			index = 58;
		}

		if((index < 59) && (i < 59) && (index >= 0)) {
			if(index < 58) {
			//	printf("iiiiii\n");
				block->next->prev = block->prev;
				block->prev->next = block->next;
				
				header * split = &freelistSentinels[index];
				header * sn = split->next;
				split->next = block;
				block->next = sn;
				block->prev = &freelistSentinels[index];
				block->next->prev = block;
				block->prev->next = block;
			} else if (index == 58) {
//				block->next->left_size = newsize;
		//		block->next->prev = block;
		//		block->prev->next = block;
				//	printf("eeee\n");
			}
		}

//		printf("bbbbb\n");
		//temp += get_size(newH);
		//header * rH = (header *) temp;
		//set_size(rH->prev, asize);

		hdr = newH;
//		printf("break\n");
//		break;
	} /*else if(flag = 2) {
	}*/
//	block = block->next;
 // }

//task1-


// printf("return\n");
  return (header *) hdr->data;
//  assert(false);
//  exit(1);
}

/**
 * @brief Helper to get the header from a pointer allocated with malloc
 *
 * @param p pointer to the data region of the block
 *
 * @return A pointer to the header of the block
 */
static inline header * ptr_to_header(void * p) {
  return (header *)((char *) p - ALLOC_HEADER_SIZE); //sizeof(header));
}

/**
 * @brief Helper to manage deallocation of a pointer returned by the user
 *
 * @param p The pointer returned to the user by a call to malloc
 */
static inline void deallocate_object(void * p) {
  //(void) p;
  //assert(false);
  //exit(1);

		
	if(p == NULL) {
		return;
	}

	header * hdr = ptr_to_header(p);
//	enum state hs = get_state(hdr);
	header * left = get_left_header(hdr);
	header * right = get_right_header(hdr);
	enum state ls = get_state(left);
	enum state rs = get_state(right);
	enum state hs = get_state(hdr);

//	printf("do\n");
	if(hs == 0) {

		//printf("Double Free Detected\n");
		//printf("Assertion Failed!\n");
		//assert(false);
		//return;
		const char * msg = "Double Free Detected\nAssertion Failed!\n";
		write(2, msg, strlen(msg));
		exit(1);
	}

	if(ls != 0 && rs != 0) {
//		printf("1\n");
		size_t hss = get_size(hdr);
		size_t index = ((hss-(ALLOC_HEADER_SIZE))/8) - 1;
		index = (int)index;
		if(index > 58) {
			index = 58;
		}
		header * freelist = &freelistSentinels[index];
		//header * fl;
	//	header * fr;
		header * bb = freelist->next;
//		printf("567\n");
		/*		
		while(bb != freelist) {
			if(get_state(bb) == 2) {
				break;
			}
			bb = bb->next;
		}
*/
		//printf("index = %ld\n", index);
		set_state(hdr, 0);
		//hdr->size_state = (hdr->size_state & ~0x3) | 0;
//		printf("579\n");
		freelist->next = hdr;
//		printf("581\n");
		hdr->next = bb;
//		printf("583\n");
		hdr->prev = &freelistSentinels[index];
//		printf("585\n");
		hdr->next->prev = hdr;
//		printf("587\n");
		hdr->prev->next = hdr;
//		printf("589\n");
	} else if (ls == 0 && rs != 0) {
//		printf("2\n");
		size_t leftSize = get_size(left);
		size_t hSize = get_size(hdr);
		size_t ss = leftSize + hSize;
		set_size(left, ss);
		set_size(hdr, 0);
		set_state(hdr, 0);
		right->left_size = ss;
	
		size_t hi = ((leftSize - ALLOC_HEADER_SIZE)/8) - 1;
		hi = (int) hi;

		size_t index = ((ss - ALLOC_HEADER_SIZE)/8) - 1;
		index = (int) index;

		if(hi > 58) {
			hi = 58;
		}
		if(index > 58) {
			index = 58;
		}
		if((hi < 59) && (index < 59)) {
			if(hi != 58) {
				header * lf = &freelistSentinels[hi];
				header * block = lf->next;
	
				while(block != lf) {
					if(block == left) {
						break;
					}
					block = block->next;
				}

				block->next->prev = block->prev;
				block->prev->next = block->next;
		
				header * freelist = &freelistSentinels[index];
				header * bb = freelist->next;
				freelist->next = left;
				left->next = bb;
				left->prev = &freelistSentinels[index];
				left->next->prev = left;
				left->prev->next = left;
			} else if(hi == 58) {
//				left->next->left_size = ss;				
			}
		}
	} else if (ls != 0 && rs == 0) {
//		printf("3\n");
		size_t hSize = get_size(hdr);
		size_t rSize = get_size(right);
		size_t ss = hSize + rSize;
		set_size(hdr, ss);
		set_state(hdr, 0);
		header * rr = get_right_header(right);
		set_size(right, 0);
		
	//	header * rr = get_right_header(right);
		rr->left_size = ss;
		
		size_t ci = ((rSize - ALLOC_HEADER_SIZE)/8) - 1;
		ci = (int) ci;

		size_t si = ((ss - ALLOC_HEADER_SIZE)/8) - 1;
		si = (int) si;

		if(ci > 58) {
			ci = 58;
		}
		if(si > 58) {
			si = 58;
		}
		if((ci < 59) && (si < 59)) {
			if(ci != 58) {
				header * cf = &freelistSentinels[ci];
				header * block = cf->next;
	
				while(block != cf) {
					if(block == right) {
						break;
					}
					block = block->next;
				}
	
				block->next->prev = block->prev;
				block->prev->next = block->next;
	
				header * freelist = &freelistSentinels[si];
				header * bb = freelist->next;
				freelist->next = hdr;
				hdr->next = bb;
				hdr->prev = &freelistSentinels[si];
				hdr->next->prev = hdr;
				hdr->prev->next = hdr;
			} else if(ci == 58) {
				right = hdr;
//				right->next->left_size = ss;
			}
		}
	} else if (ls == 0 && rs == 0) {
//		printf("4\n");
		size_t lsize = get_size(left);
		size_t hsize = get_size(hdr);
		size_t rsize = get_size(right);
		size_t sum = lsize + hsize + rsize;
		header * rr = get_right_header(right);
		set_size(left, sum);
		set_size(hdr, 0);
		set_state(hdr, 0);
		set_size(right, 0);
		rr->left_size = sum;

		size_t li = ((lsize - ALLOC_HEADER_SIZE)/8) - 1;
		li = (int) li;

		size_t si = ((sum - ALLOC_HEADER_SIZE)/8) - 1;
		si = (int) si;

		right->next->prev = right->prev;
		right->prev->next = right->next;
		if(li > 58) {
			li = 58;
		}
		if(si > 58) {
			si = 58;
		}
		if((si < 59)) {
			if(li != 58) {
				left->next->prev = left->prev;
				left->prev->next = left->next;
				//right->next->prev = right->prev;
				//right->prev->next = right->next;

				header * freelist = &freelistSentinels[si];
				header * bb = freelist->next;
				freelist->next = left;
				left->next = bb;
				left->prev = &freelistSentinels[si];
				left->next->prev = left;
				left->prev->next = left;
			} else if(li == 58) {
//				left->next->left_size = sum;
			}
		}

	}

//	printf("return\n");
	return;
}

/**
 * @brief Helper to detect cycles in the free list
 * https://en.wikipedia.org/wiki/Cycle_detection#Floyd's_Tortoise_and_Hare
 *
 * @return One of the nodes in the cycle or NULL if no cycle is present
 */
static inline header * detect_cycles() {
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    for (header * slow = freelist->next, * fast = freelist->next->next; 
         fast != freelist; 
         slow = slow->next, fast = fast->next->next) {
      if (slow == fast) {
        return slow;
      }
    }
  }
  return NULL;
}

/**
 * @brief Helper to verify that there are no unlinked previous or next pointers
 *        in the free list
 *
 * @return A node whose previous and next pointers are incorrect or NULL if no
 *         such node exists
 */
static inline header * verify_pointers() {
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    for (header * cur = freelist->next; cur != freelist; cur = cur->next) {
      if (cur->next->prev != cur || cur->prev->next != cur) {
        return cur;
      }
    }
  }
  return NULL;
}

/**
 * @brief Verify the structure of the free list is correct by checkin for 
 *        cycles and misdirected pointers
 *
 * @return true if the list is valid
 */
static inline bool verify_freelist() {
  header * cycle = detect_cycles();
  if (cycle != NULL) {
    fprintf(stderr, "Cycle Detected\n");
    print_sublist(print_object, cycle->next, cycle);
    return false;
  }

  header * invalid = verify_pointers();
  if (invalid != NULL) {
    fprintf(stderr, "Invalid pointers\n");
    print_object(invalid);
    return false;
  }

  return true;
}

/**
 * @brief Helper to verify that the sizes in a chunk from the OS are correct
 *        and that allocated node's canary values are correct
 *
 * @param chunk AREA_SIZE chunk allocated from the OS
 *
 * @return a pointer to an invalid header or NULL if all header's are valid
 */
static inline header * verify_chunk(header * chunk) {
	if (get_state(chunk) != FENCEPOST) {
		fprintf(stderr, "Invalid fencepost\n");
		print_object(chunk);
		return chunk;
	}
	
	for (; get_state(chunk) != FENCEPOST; chunk = get_right_header(chunk)) {
		if (get_size(chunk)  != get_right_header(chunk)->left_size) {
			fprintf(stderr, "Invalid sizes\n");
			print_object(chunk);
			return chunk;
		}
	}
	
	return NULL;
}

/**
 * @brief For each chunk allocated by the OS verify that the boundary tags
 *        are consistent
 *
 * @return true if the boundary tags are valid
 */
static inline bool verify_tags() {
  for (size_t i = 0; i < numOsChunks; i++) {
    header * invalid = verify_chunk(osChunkList[i]);
    if (invalid != NULL) {
      return invalid;
    }
  }

  return NULL;
}

/**
 * @brief Initialize mutex lock and prepare an initial chunk of memory for allocation
 */
static void init() {
  // Initialize mutex for thread safety
  pthread_mutex_init(&mutex, NULL);

#ifdef DEBUG
  // Manually set printf buffer so it won't call malloc when debugging the allocator
  setvbuf(stdout, NULL, _IONBF, 0);
#endif // DEBUG

  // Allocate the first chunk from the OS
  header * block = allocate_chunk(ARENA_SIZE);

  header * prevFencePost = get_header_from_offset(block, -ALLOC_HEADER_SIZE);
  insert_os_chunk(prevFencePost);

  lastFencePost = get_header_from_offset(block, get_size(block));

  // Set the base pointer to the beginning of the first fencepost in the first
  // chunk from the OS
  base = ((char *) block) - ALLOC_HEADER_SIZE; //sizeof(header);

  // Initialize freelist sentinels
  for (int i = 0; i < N_LISTS; i++) {
    header * freelist = &freelistSentinels[i];
    freelist->next = freelist;
    freelist->prev = freelist;
  }

  // Insert first chunk into the free list
  header * freelist = &freelistSentinels[N_LISTS - 1];
  freelist->next = block;
  freelist->prev = block;
  block->next = freelist;
  block->prev = freelist;
}

/* 
 * External interface
 */
void * my_malloc(size_t size) {
  pthread_mutex_lock(&mutex);
  header * hdr = allocate_object(size); 
  pthread_mutex_unlock(&mutex);
  return hdr;
}

void * my_calloc(size_t nmemb, size_t size) {
  return memset(my_malloc(size * nmemb), 0, size * nmemb);
}

void * my_realloc(void * ptr, size_t size) {
  void * mem = my_malloc(size);
  memcpy(mem, ptr, size);
  my_free(ptr);
  return mem; 
}

void my_free(void * p) {
  pthread_mutex_lock(&mutex);
  deallocate_object(p);
  pthread_mutex_unlock(&mutex);
}

bool verify() {
  return verify_freelist() && verify_tags();
}
