// make-2 : make second copy of image at 0x1400000 and fill the rest up to 128Mb
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int make2(const char* dst,const char* src) {
	FILE *fs=0, *fd=0;
	enum { 
		page_size =2048+64,
		copy_ofs  =0x1400000/2048*page_size,
		total_size=0x8000000/2048*page_size,
		buf2_size =page_size 
	 };
	int src_size,pos,rd,rc; char *buf1=0, buf2[buf2_size];
	
	fs=fopen(src,"rb"); if (!fs) { fprintf(stderr,"unable to open %s\n",src); rc=1; goto leave; }
	fd=fopen(dst,"wb+"); if (!fd) { fprintf(stderr,"unable to create %s\n",dst); fclose(fs); rc=2;goto leave; }
	fseek(fs,0,SEEK_END); src_size=ftell(fs); fseek(fs,0,SEEK_SET);
	printf("source size %08X (%d)\n",src_size,src_size);
	printf("copy offset %08X\n",copy_ofs);
	buf1=malloc(src_size); if (!buf1) { fprintf(stderr,"unable allocate %d bytes\n",src_size); rc=3; goto leave; }
	printf("reading source\n");
	rd=fread(buf1,1,src_size,fs); if (rd!=src_size) { fprintf(stderr,"read fail %d!=%d\n",src_size,rd); rc=4; goto leave; }
	if (rd % page_size ) { fprintf(stderr,"source size %d must be multiplier of %d\n",rd,page_size); rc=5; goto leave; }
	if (rd > copy_ofs  ) { fprintf(stderr,"source size 0x%08X must be less than copy offset 0x%08X\n",rd,copy_ofs); rc=6; goto leave; }
	memset(buf2,0xFF,buf2_size);
	printf("write 1st copy\n");
	pos=fwrite(buf1,1,rd,fd); 
	printf("0x%08X padding\n",pos/page_size*2048);
	for(;pos<copy_ofs;pos+=buf2_size) fwrite(buf2,1,buf2_size,fd);
	printf("0x%08X write 2st copy\n",pos/page_size*2048);
	pos+=fwrite(buf1,1,rd,fd);
	printf("0x%08X padding\n",pos/page_size*2048);
	for(;rd<total_size;rd+=buf2_size) fwrite(buf2,1,buf2_size,fd);
	printf("0x%08X done size=0x%08X\n",pos/page_size*2048,pos);
	rc=0;
leave:
	free(buf1);	
	fclose(fd);
	fclose(fs);
	return 0;
}

int main(int argc,char** argv) {
	if (argc<2) {
		fprintf(stderr,"usage: make-2 src dst\n");
		return -1;
	}
	return make2(argv[2],argv[1]);
}
