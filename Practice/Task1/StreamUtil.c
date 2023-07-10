#include "StreamUtil.h"

void replaceInStream(
	char* block, int blockSize, char* query, int querySize, char* to, int toSize,
	FILE* outStream
)
{
	static int queryPos = 0;

	for (int i = 0; i < blockSize; i++) {
		if (block[i] == query[queryPos]) {
			queryPos++;
			if (queryPos == querySize) {
				fwrite(to, sizeof(char), toSize, outStream);
				queryPos = 0;
			}
		}
		else {
			if (queryPos > 0) {
				fwrite(query, sizeof(char), queryPos, outStream);
			}
			queryPos = 0;
			fwrite(block + i, sizeof(char), 1, outStream); 
		}
	}
}

//void replaceInStreamFast(
//	char* block, int blockSize, char* query, int querySize, char* to, int toSize,
//	FILE* outStream
//)
//{
//	if (querySize <= blockSize) 
//		exit(printf("replaceInStreamFast: You can call this func if, and only if, querySize > blockSize"));
//
//	static int queryPos = 0;
//	int lastFail = 0;
//	int lastStart = 0;
//	int lastPos = queryPos;
//	for (int i = 0; i < blockSize; i++) {
//		if (block[i] == query[queryPos]) {
//			
//			if (queryPos == 0 && lastPos == 0) lastStart = i;
//			queryPos++;
//			if (queryPos == querySize) {
//				fwrite(to, sizeof(char), toSize, outStream);
//				queryPos = 0;
//				lastPos = 0;
//				lastFail = i+1;
//			}
//		}
//		else {
//			if (queryPos > 0) {
//				if(lastPos > 0) fwrite(query, sizeof(char), queryPos, outStream);
//				if(lastStart == 0) lastFail = i;
//				lastStart = 0;
//			}
//
//			queryPos = 0;
//			lastPos = 0;
//			
//		}
//	}
//
//	if (queryPos > 0) blockSize = lastStart;
//	fwrite(block + lastFail, sizeof(char), (size_t)blockSize - lastFail, outStream);
//	
//}