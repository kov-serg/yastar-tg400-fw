// mkecc : add oob with hamming ecc
#include <stdio.h>
#include <string.h>

static unsigned char par(unsigned char x) { x^=x>>4; x^=x>>2; x^=x>>1; return x&1; }
void ecc(unsigned char* res, unsigned char* data,int size) {
	static const unsigned char mask[6]={0x55,0xAA,0x33,0xCC,0x0F,0xF0};
	int i,k; enum { pn=24 }; unsigned char p[pn],x;
	for(i=0;i<pn;++i) p[i]=1;
	for(i=0;i<size;i++) {
		x=data[i];
		for(k=0;k<6;k++) p[k]^=par(x&mask[k]);
		for(k=0;k<9;k++) p[6+2*k+((i>>k)&1)]^=par(x);
	}
	res[0]=(p[21]<<7)|(p[20]<<6)|(p[19]<<5)|(p[18]<<4)|(p[17]<<3)|(p[16]<<2)|(p[15]<<1)|p[14];
	res[1]=(p[13]<<7)|(p[12]<<6)|(p[11]<<5)|(p[10]<<4)|(p[ 9]<<3)|(p[ 8]<<2)|(p[ 7]<<1)|p[ 6];
	res[2]=(p[ 5]<<7)|(p[ 4]<<6)|(p[ 3]<<5)|(p[ 2]<<4)|(p[ 1]<<3)|(p[ 0]<<2)|(p[23]<<1)|p[22];
}
int ecc2x(unsigned char* ecc,int group) { // returns wrong bit position (grp=0,1)
	static const unsigned char t[12]={ 18,20,22, 8,10,12,14, 0,2,4,6, 16 };
	int r,i; unsigned char k;
	r=group?0xFFF:0;
	for(i=0;i<12;i++) { k=t[i]^group; if (ecc[k>>3]&(1<<(k&7))) r^=1<<i; }
	return r;
}

int create_file(const char* dst,const char* src) {
	enum { 
		page_size=2048, 
		oob_size=64, 
		buf_size=page_size+oob_size,
		ecc_ofs=page_size+oob_size-24 
	};
	FILE *fs, *fd; int rc,i,rd,wr;
	unsigned char buf[buf_size];
	fs=fopen(src,"rb");  if (!fs) { fprintf(stderr,"unable to open %s\n",src); return 1; }
	fd=fopen(dst,"wb+"); if (!fd) { fprintf(stderr,"unable to create %s\n",dst); fclose(fs); return 2; }
	rc=0;
	do {
		rd=fread(buf,1,page_size,fs); if (rd<=0) break;
		if (rd>0 && rd<page_size) memset(buf+rd,0xFF,page_size-rd);
		memset(buf+page_size,0xFF,oob_size);
		for(i=0;i<8;i++) ecc(buf+ecc_ofs+3*i,buf+i*256,256);
		wr=fwrite(buf,1,buf_size,fd);
		if (wr!=buf_size) { fprintf(stderr,"write error\n"); rc=3; break; }
	} while(rd==page_size);
	fclose(fd);
	fclose(fs);
	return rc;
}

int main(int argc,char** argv) {
	if (argc<2) {
		fprintf(stderr,"usage: make-ecc src dst\n");
		return -1;
	}
	return create_file(argv[2],argv[1]);
}
