#ifndef MEMINFO_H
# define MEMINFO_H

#include <stdint.h>

/* Structure containing all the members that might exists
   in /proc/meminfo pseudo file. */
struct proc_meminfo {
	uint64_t mem_total;
	uint64_t mem_free;
	uint64_t mem_avail;
	uint64_t buffers;
	uint64_t cached;
	uint64_t swap_cached;
	uint64_t active;
	uint64_t inactive;
	uint64_t active_anon;
	uint64_t inactive_anon;
	uint64_t active_file;
	uint64_t inactive_file;
	uint64_t unevictable;
	uint64_t mlocked;
        uint64_t high_total;
	uint64_t high_free;
	uint64_t low_total;
	uint64_t low_free;
	uint64_t mmap_copy;
	uint64_t swap_total;
	uint64_t swap_free;
	uint64_t zswap;
	uint64_t zswapped;
        uint64_t dirty;
	uint64_t write_back;
	uint64_t anon_pages;
	uint64_t mapped;
	uint64_t shmem;
	uint64_t k_reclaimable;
	uint64_t slab;
	uint64_t s_reclaimable;
	uint64_t s_unreclaim;
	uint64_t kernel_stack;
	uint64_t page_tables;
	uint64_t quick_lists;
	uint64_t sec_page_tables;
	uint64_t nfs_unstable;
	uint64_t bounce;
	uint64_t write_back_tmp;
	uint64_t commit_limit;
	uint64_t committed_as;
	uint64_t vm_alloc_total;
	uint64_t vm_alloc_used;
	uint64_t vm_alloc_chunk;
	uint64_t percpu;
	uint64_t hardware_corrupted;
	uint64_t lazy_free;
	uint64_t anon_huge_pages;
	uint64_t shmem_huge_pages;
	uint64_t shmempmd_mapped;
        uint64_t file_huge_pages;
	uint64_t filepmd_mapped;	
        uint64_t cma_total;
	uint64_t cma_free;
	uint64_t huge_pages_total;
	uint64_t huge_pages_free;
	uint64_t huge_pages_rsvd;
	uint64_t huge_pages_surp;
	uint64_t huge_page_size;
	uint64_t hugetlb;

#if defined (__i386__) || defined (__x86_64__)
	uint64_t direct_map_4k;
	uint64_t direct_map_4M;
	uint64_t direct_map_2M;
	uint64_t direct_map_1G;
#endif	
};

#ifndef PROC_MEMINFO_PATH
# define PROC_MEMINFO_PATH    "/proc/meminfo"
#endif

/* Constants. Used as return codes. */
#ifndef PROC_MEMINFO_ALL_OKAY
# define PROC_MEMINFO_ALL_OKAY         (0)
#endif
#ifndef PROC_MEMINFO_OPEN_FAILED
# define PROC_MEMINFO_OPEN_FAILED      (-1)
#endif
#ifndef PROC_MEMINFO_ALLOC_FAILED
# define PROC_MEMINFO_ALLOC_FAILED     (-2)
#endif
#ifndef PROC_MEMINFO_NO_KEY
# define PROC_MEMINFO_NO_KEY           (-3)
#endif

#ifdef MEMINFO_IMPL

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
	
/* Compiler extension to check for overflow. */
#if defined (__GNUC__)
# define gcc_mul_ovf_check(x, y, r)		\
	__builtin_mul_overflow(x, y, &r)	
#else
/* For compilers that doesn't implements GNU extensions.
   e.g. TinyCC */
# define gcc_mul_ovf_check(x, y, r)     (0)
#endif

int proc_do_init_meminfo(struct proc_meminfo *pmi, char **p)
{
	int fd;
	char buf[1024];
	ssize_t nbytes_read;
	size_t total_read;

	/* Initialize the struct members with 0. */
	memset(pmi, '\0', sizeof(struct proc_meminfo));

	if ((fd = open(PROC_MEMINFO_PATH, O_RDONLY)) == -1)
	        return (PROC_MEMINFO_OPEN_FAILED);

	memset(buf, '\0', sizeof(buf));
        if ((*p = calloc(1, sizeof(char))) == NULL)
		return (PROC_MEMINFO_ALLOC_FAILED);

        total_read = 0;
	while ((nbytes_read = read(fd, buf, sizeof(buf) - 1)) > 0) {
		if ((*p = realloc(*p, total_read + nbytes_read + 1)) == NULL)
		        return (PROC_MEMINFO_ALLOC_FAILED);

	        memcpy(*p + total_read, buf, nbytes_read);
		total_read += nbytes_read;
	}

        if (*(*p + total_read - 1) == '\n')
		*(*p + total_read - 1) = '\0';
	else if (*(*p + total_read) == '\n')
		*(*p + total_read) = '\0';

	return (PROC_MEMINFO_ALL_OKAY);
}

int proc_do_get_kv(const char *src, const char *key, uint64_t *dst)
{
	char *k;
	unsigned int r;

        if ((k = strstr(src, key)) == NULL)
		return (PROC_MEMINFO_NO_KEY);

	/* Skip everything until we found a whitespace. */
	for (; *k != ' '; k++)
		;

	/* Skip everything (including whitespaces) until we
	   found a digit. */
	for (; !isdigit(*k); k++)
		;

        for (; *k; k++) {
		if (!isdigit(*k))
			break;
	        if (gcc_mul_ovf_check(*dst, 10 + *k - '0', r))
			return (-3);

		*dst = *dst * 10 + (unsigned)*k - '0';
	}
	
	return (PROC_MEMINFO_ALL_OKAY);
}

void proc_do_collect_all(struct proc_meminfo *pmi, const char *src)
{
	proc_do_get_kv(src, "MemTotal:", &pmi->mem_total);
	proc_do_get_kv(src, "MemFree:", &pmi->mem_free);
	proc_do_get_kv(src, "MemAvailable:", &pmi->mem_avail);
	proc_do_get_kv(src, "Buffers:", &pmi->buffers);
	proc_do_get_kv(src, "Cached:", &pmi->cached);
	proc_do_get_kv(src, "SwapCached:", &pmi->swap_cached);
	proc_do_get_kv(src, "Active:", &pmi->active);	
	proc_do_get_kv(src, "Inactive:", &pmi->inactive);
	proc_do_get_kv(src, "Active(anon)", &pmi->active_anon);
	proc_do_get_kv(src, "Inactive(anon):", &pmi->inactive_anon);
	proc_do_get_kv(src, "Active(file):", &pmi->active_file);
	proc_do_get_kv(src, "Inactive(file):", &pmi->inactive_file);
	proc_do_get_kv(src, "Unevictable:", &pmi->unevictable);
	proc_do_get_kv(src, "Mlocked:", &pmi->mlocked);
	proc_do_get_kv(src, "HighTotal:", &pmi->high_total);
	proc_do_get_kv(src, "HighFree:", &pmi->high_free);
	proc_do_get_kv(src, "LowTotal:", &pmi->low_total);
	proc_do_get_kv(src, "LowFree:", &pmi->low_free);
	proc_do_get_kv(src, "MmapCopy:", &pmi->mmap_copy);
	proc_do_get_kv(src, "SwapTotal:", &pmi->swap_total);
	proc_do_get_kv(src, "SwapFree:", &pmi->swap_free);
	proc_do_get_kv(src, "Zswap:", &pmi->zswap);
	proc_do_get_kv(src, "Zswapped:", &pmi->zswapped);
	proc_do_get_kv(src, "Dirty:", &pmi->dirty);
	proc_do_get_kv(src, "Writeback:", &pmi->write_back);
	proc_do_get_kv(src, "AnonPages:", &pmi->anon_pages);
	proc_do_get_kv(src, "Mapped:", &pmi->mapped);
	proc_do_get_kv(src, "Shmem:", &pmi->shmem);
	proc_do_get_kv(src, "KReclaimable:", &pmi->k_reclaimable);
	proc_do_get_kv(src, "Slab:", &pmi->slab);
	proc_do_get_kv(src, "SReclaimable:", &pmi->s_reclaimable);
	proc_do_get_kv(src, "SUnreclaim:", &pmi->s_unreclaim);
	proc_do_get_kv(src, "KernelStack:", &pmi->kernel_stack);
	proc_do_get_kv(src, "PageTables:", &pmi->page_tables);
	proc_do_get_kv(src, "QuickLists:", &pmi->quick_lists);
	proc_do_get_kv(src, "SecPageTables:", &pmi->sec_page_tables);
	proc_do_get_kv(src, "NFS_Unstable:", &pmi->nfs_unstable);
	proc_do_get_kv(src, "Bounce:", &pmi->bounce);
	proc_do_get_kv(src, "WritebackTmp:", &pmi->write_back_tmp);
	proc_do_get_kv(src, "CommitLimit:", &pmi->commit_limit);
	proc_do_get_kv(src, "Committed_AS:", &pmi->committed_as);
	proc_do_get_kv(src, "VmallocTotal:", &pmi->vm_alloc_total);
	proc_do_get_kv(src, "VmallocUsed:", &pmi->vm_alloc_used);
	proc_do_get_kv(src, "VmallocChunk:", &pmi->vm_alloc_chunk);
	proc_do_get_kv(src, "Percpu:", &pmi->percpu);
	proc_do_get_kv(src, "HardwareCorrupted:", &pmi->hardware_corrupted);
	proc_do_get_kv(src, "LazyFree:", &pmi->lazy_free);
	proc_do_get_kv(src, "AnonHugePages:", &pmi->anon_huge_pages);
	proc_do_get_kv(src, "ShmemHugePages:", &pmi->shmem_huge_pages);
	proc_do_get_kv(src, "ShmemPmdMapped:", &pmi->shmempmd_mapped);
	proc_do_get_kv(src, "FileHugePages:", &pmi->file_huge_pages);
	proc_do_get_kv(src, "FilePmdMapped:", &pmi->filepmd_mapped);
	proc_do_get_kv(src, "HugePages_Total:", &pmi->huge_pages_total);
	proc_do_get_kv(src, "HugePages_Free:", &pmi->huge_pages_free);
	proc_do_get_kv(src, "HugePages_Rsvd:", &pmi->huge_pages_rsvd);
	proc_do_get_kv(src, "HugePages_Surp:", &pmi->huge_pages_surp);
	proc_do_get_kv(src, "Hugepagesize:", &pmi->huge_page_size);
	proc_do_get_kv(src, "Hugetlb:", &pmi->hugetlb);

	/* According to proc(5) these are only available for x86 CPUs. */
#if defined (__i386__) || defined (__x86_64__)
	proc_do_get_kv(src, "DirectMap4k:", &pmi->direct_map_4k);
	proc_do_get_kv(src, "DirectMap2M:", &pmi->direct_map_2M);
	proc_do_get_kv(src, "DirectMap4M:", &pmi->direct_map_4M);
	proc_do_get_kv(src, "DirectMap1G:", &pmi->direct_map_1G);
#endif /* __i386__, __x86_64__ */
	
}

#endif /* MEMINFO_IMPL */

#endif /* MEMINFO_H */
