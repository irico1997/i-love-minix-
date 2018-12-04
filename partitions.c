
#include "header.h"
#define PART 0
#define SUBPART 1

void check_err(int err);
void pusage();
void set_base(int offset);

/* command line values */
extern int part;
extern int sub_part;
extern int verbose;
extern char *imagefile;
extern char *path;
extern char *srcpath;
/* important stuff */
extern FILE *fs;
extern uint32_t zone_size;
extern superblock s;
extern long base;
extern p_entry partition;
extern p_entry subpartition;

void check_pvalues()
{
   if(part == -1 && sub_part == -1)
   {
      return;
   }
   if(sub_part != -1 && part == -1)
   {
      pusage();
   }
   if(sub_part > 3 || part > 3 || part < 0 || sub_part < 0)
   {
      fprintf(stderr, "Partition number must be between 0 and 3\n");
      exit(EXIT_FAILURE);
   }
}
/* verify that the partition is valid */
void check_psig()
{
   uint8_t signature510 = 0x55;
   uint8_t signature511 = 0xAA;
   uint8_t actual510;
   uint8_t actual511;
   check_err(fseek(fs, base + PART_SIGLOC, SEEK_SET));
   fread(&actual510, sizeof(signature510), 1, fs);
   fread(&actual511, sizeof(signature511), 1, fs);
   if(actual510 != signature510 || actual511 != signature511)
   {
      fprintf(stderr, "Signature not valid\n");
      exit(EXIT_FAILURE);
   }
}

void check_partition(p_entry partition)
{
   /* check validity */
   if(partition.type != 0x81)
   {
      fprintf(stderr, "Not a minix partition\n");
      exit(EXIT_FAILURE);
   }
   /* use lfirst to read in super block from first sector */
}

void load_pt(p_entry *part_loc, int part_num)
{
   p_entry pt[4];
   check_err(fseek(fs, base + 0x1BE, SEEK_SET));
   fread(pt, sizeof(p_entry)*4, 1, fs);
   *part_loc = pt[part];
   check_partition(*part_loc);
   /* update base */
   set_base(pt[part].lFirst * 512);
}

void check_partitions()
{
   /* unpartitioned - set up superblock and continue */
   if(part == -1)
   {
      return;
   }
   check_psig();
   load_pt(&partition, part);
   if(sub_part == -1)
   {
      return;
   }
   check_psig();
   load_pt(&subpartition, sub_part);
}
