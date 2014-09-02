#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define BUFSIZE 4194304

int main(int argc, char **argv)
{


    unsigned char *buf = malloc(BUFSIZE);
    if (buf == NULL) {
	printf("Could not allocate buffer.\n");
	return 1;
    }

    int outf;
    char outf_name[] = "temp_hashname_XXXXXX";
    outf = mkstemp(outf_name);

    if (outf == -1) {
	perror("Creating temporary file");
	return 1;
    }

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    ssize_t rret = 0;
    ssize_t rcount = 0;
    for (;;) {

	rret = read(STDIN_FILENO, buf + rcount, 4096);

	if (rret == -1) {
	    perror("Reading from stdin");
	    return 1;
	}

	rcount += rret;
	if ((rcount > (BUFSIZE - 8192)) || (rret == 0)) {

	    int wret = 0;
	    int wcount = 0;
	    do {
		wret = write(outf, buf + wcount, rcount - wcount);
		if (wret == -1) {
		    perror("Writing output file");
		    return 1;
		}
		wcount += wret;
	    }
	    while (wcount < rcount);

	    SHA256_Update(&ctx, buf, rcount);
	    rcount = 0;
	}

	if (rret == 0) {
	    break;
	}



    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned char *h = hash;
    char hash_name[1024];
    char *p = hash_name;

    SHA256_Final(hash, &ctx);

    int i;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
	sprintf(p, "%02x", *h++);
	p += 2;
    }
    hash_name[32] = '\0';

    printf("%s\n", hash_name);
    if (argc > 1) {
	strncat(hash_name, argv[1], 128);
    }

    int ret = rename(outf_name, hash_name);

    if (ret == -1) {
	perror("Renaming temp file");
    }
    close(outf);
    return ret;
}
