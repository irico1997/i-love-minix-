#include "header.h"

/* command line values */
int part = -1;
int sub_part = -1;
int verbose = 0;
char *imagefile = NULL;
char *path = NULL;
char *srcpath = NULL;
/* important stuff */
FILE *fs = NULL;
uint32_t zone_size = -1;
superblock s;
long base = -1;
p_entry partition;
p_entry subpartition;
uint32_t end;
char pad[6] = "     ";

void pusage()
{
   printf("usage: minls [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n");
   exit(EXIT_FAILURE);
}

void seek_zone(uint32_t zone)
{
   /* seek zone # * blocks per zone */
}

void get_dir_entry()
{
   /* read sizeof(entry) more bytes */
}

void search_dir(uint32_t inode)
{
   char filename[61];
   filename[60] = '\0';
   /* seek_zone(num) */
   /* fseek to start of zone 1
    * read until pointer is at end of zone 1
    * move to zone 2 and repeat
    * after zone 7, do subzones (read uint32_t zone_num starting from indirect addr)
    */
}

void read_command_line(int argc, char *argv[], int function)
{
   int i = 1;
   if(argc < 2)
   {
      pusage();
   }
   while(i < argc)
   {
      if(strcmp("-v", argv[i]) == 0)
      {
         verbose = 1;
         i++;
      }
      else if(strcmp("-p", argv[i]) == 0)
      {
         if(i + 1 == argc)
         {
            pusage();
         }
         if(sscanf(argv[i+1], "%d", &part) == 0)
         {
            pusage();
         }
         i += 2;
         
      }
      else if(strcmp("-s", argv[i]) == 0)
      {
         if(i + 1 == argc)
         {
            pusage();
         }
         if(sscanf(argv[i+1], "%d", &sub_part) == 0)
         {
            pusage();
         }
         i += 2;
      }
      else if(imagefile == NULL)
      {
         imagefile = argv[i];
         i++;
      }
      else if(function == MINGET && srcpath == NULL)
      {
         srcpath = argv[i];
         i++;
      }
      else if(path != NULL)
      {
         path = argv[i];
         i++;
      }
      else
      {
         pusage();
      }
   }
   check_pvalues();
}

void check_err(int err)
{
   /* my fave error checker */
   if(err)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
}

void set_base(int offset)
{
   check_err(fseek(fs, offset, SEEK_SET));
   base = ftell(fs);
   check_err(base);
}

FILE *open_filesystem()
{
   fs = fopen(imagefile, "r");
   if(fs == NULL)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   set_base(0);
   return fs;
}

void get_superblock()
{
   check_err(fseek(fs, base + SB_LOC, SEEK_SET));
   fread(&s, sizeof(s), 1, fs);
}

void print_part(p_entry *p)
{
   printf("%sbootind: %u\n"\
         "%sstart_head: %u\n"\
         "%sstart_sec: %u\n"\
         "%sstart_cyl: %u\n"\
         "%stype: %u\n"\
         "%send_head: %u\n"\
         "%send_sec: %u\n"\
         "%send_cyl: %u\n"\
         "%slFirst: %u\n"\
         "%ssize: %u\n", \
         pad, p->bootind, pad, p->start_head, pad, p->start_sec, pad,\
         p->start_cyl, pad, p->type, pad, p->end_head, pad, p->end_sec,\
         pad, p->end_cyl, pad, p->lFirst, pad, p->size);
}

void print_sb()
{
   printf("Superblock:\n");
   printf("%sninodes %u\n"\
         "%spad1: %u\n"\
         "%si_blocks: %d\n"\
         "%sz_blocks: %d\n"\
         "%sfirstdata: %u\n"\
         "%slog_zone_size: %d\n"\
         "%spad2: %d\n"\
         "%smax_file: %u\n"\
         "%szones: %u\n"\
         "%smagic: %d\n" \
         "%spad3: %d\n" \
         "%sblocksize: %u\n" \
         "%ssubversion: %u\n", \
         pad, s.ninodes, pad, s.pad1, pad, s.i_blocks, pad, s.z_blocks,\
         pad, s.firstdata, pad, s.log_zone_size, pad, s.pad2, pad, s.max_file,\
         pad, s.zones, pad, s.magic, pad, s.pad3, pad, s.blocksize,\
         pad, s.subversion);
}

void printinfo()
{
   if(part != -1)
   {
      printf("Partition:\n");
      print_part(&partition);
   }
   if(sub_part != -1)
   {
      printf("Subpartition:\n");
      print_part(&subpartition);
   }
   print_sb();
}

int main(int argc, char *argv[])
{
   read_command_line(argc, argv, MINLS);
   open_filesystem();
   check_partitions();
   get_superblock();
   printinfo();
   return 0;
}
