#include "StreamUtil.h"
#include <stdlib.h>

void replaceInStream(
	char* block, int blockSize, Query* query, char* to, int toSize,
	FILE* outStream
)
{
	for (int i = 0; i < blockSize; i++) {

		if (block[i] == query->match[query->pos]) // совпадает
		{ 
			checkMatch:
			query->pos++;
			if (query->pos == query->size)
			{
				fwrite(to, sizeof(char), toSize, outStream);
				query->pos = 0;
			}
			query->isTrailing = (query->match[0] == block[i]);
		}
		else
		{
			if(block[i] == query->match[0] && query->isTrailing) //не совпало, но возможно совпадёт
			{
				fwrite(query, sizeof(char), 1, outStream); // запись одного 'trailing' символа
			}
			else
			{
				if (query->pos > 0) { //провал при совпадении => запись query до проваленной позиции
					fwrite(query, sizeof(char), query->pos, outStream);
				}   
				query->pos = 0;
				query->isTrailing = 0;
				if (block[i] == query->match[0]) {
					goto checkMatch;
				}
				fwrite(block + i, sizeof(char), 1, outStream); 
			}
		}
	}
}



#include <string.h>

Query* NewQuery(char* match)
{
	Query* q = calloc(1, sizeof(Query));
	if (q == 0) return 0;
	q->isTrailing = 0;
	q->match = match;
	q->pos = 0;
	q->size = strlen(match);

	q->trailLength = 0;

	for (int i = 1; i < q->size; i++) {
		if (match[q->trailLength] == match[i]) {
			q->trailLength++;
		}
		else {
			if (q->trailLength > 0) {
				break;
			}
		}
	}

	return q;
}


