#include <kernel/list.h>
#include <kernel/atag.h>
#include <kernel/mem.h>
#include <common/stdlib.h>
#include <stdint.h>
#include <stddef.h>

extern uint8_t __end;

static uint32_t num_pages;

DEFINE_LIST(page);
IMPLEMENT_LIST(page);

static page_t * all_pages_array;
page_list_t free_pages;

void mem_init(atag_t * atags) {
	uint32_t mem_size;
	uint32_t page_array_len;
	uint32_t kernel_pages;
	uint32_t i;

	// Get the total number of pages
	mem_size = get_mem_size(atags);
	num_pages = mem_size / PAGE_SIZE;

	// Allocate space for all those pages' metadata.
	// Start this block just after kernel image is finished
	page_array_len = sizeof(page_t) * num_pages;
	all_pages_array = (page_t *)&__end;
	bzero(all_pages_array, page_array_len);
	INITIALIZE_LIST(free_pages);

	// Iterate over all pages and mark them with appropriate flags
	// Start with kernel pages
	kernel_pages = ((uint32_t)&__end) / PAGE_SIZE;
	for (i = 0; i < kernel_pages; i++) {
		all_pages_array[i].vaddr_mapped = i * PAGE_SIZE; 	// Identitty map the kernel pages
		all_pages_array[i].flags.allocated = 1;
		all_pages_array[i].flags.kernel_page = 1;
	}

	// Map the rest of the pages as unallocated, and add them to free list
	for (; i < num_pages; i++) {
		all_pages_array[i].flags.allocated = 0;
		append_page_list(&free_pages, &all_pages_array[i]);
	}
}

void * alloc_page(void) {
	page_t * page;
	void * page_mem;

	if (size_page_list(&free_pages) == 0) {
		return 0;
	}

	// Get a free page
	page = pop_page_list(&free_pages);
	page->flags.kernel_page = 1;
	page->flags.allocated = 1;

	// Get the address the physical page refers to
	page_mem = (void *)((page - all_pages_array) * PAGE_SIZE);

	// Zero out the page, to prevent malicious users from reading all data in it
	bzero(page_mem, PAGE_SIZE);

	return page_mem;
}

void free_page(void * ptr) {
	page_t * page;

	// Get page metadata from the physical address
	page = all_pages_array + ((uint32_t)ptr / PAGE_SIZE);

	// Mark the page as free
	page->flags.allocated = 0;
	append_page_list(&free_pages, page);
}