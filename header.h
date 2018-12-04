#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "partitions.h"

#define MINLS 0
#define MINGET 1
#define PART_SIGLOC 510
#define SB_LOC 1024
#define DIRECT_ZONES 7

typedef struct Directory_Entry
{
   uint32_t inode;
   unsigned char name[60];
} d_entry;

typedef struct i_node 
{
   uint16_t mode;
   uint16_t links;
   uint16_t uid;
   uint16_t gid;
   uint32_t size;
   int32_t atime;
   int32_t mtime;
   int32_t ctime;
   uint32_t zone[DIRECT_ZONES];
   uint32_t indirect;
   uint32_t two_indirect;
   uint32_t unused;
} inode;

typedef struct Partition_Entry
{
   uint8_t bootind; 
   uint8_t start_head;
   uint8_t start_sec;
   uint8_t start_cyl;
   uint8_t type;
   uint8_t end_head;
   uint8_t end_sec;
   uint8_t end_cyl;
   uint8_t lFirst;
   uint8_t size;
} p_entry;

typedef struct Superblock
{
   uint32_t ninodes;
   uint16_t pad1;
   int16_t i_blocks;
   int16_t z_blocks;
   uint16_t firstdata;
   int16_t log_zone_size;
   int16_t pad2;
   uint32_t max_file;
   uint32_t zones;
   int16_t magic;
   int16_t pad3;
   uint16_t blocksize;
   uint8_t subversion;
} superblock;
#endif
