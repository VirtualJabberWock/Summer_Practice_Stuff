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
		}
		else
		{
			//if(block[i] == query->match[0] && query->isTrailing) //не совпало, но возможно совпадёт
			//{
			//	fwrite(query, sizeof(char), 1, outStream); // запись одного 'trailing' символа
			//}
			/*else
			{*/
			if (query->pos > 0) { //провал при совпадении => запись query до проваленной позиции
				fwrite(query, sizeof(char), query->pos, outStream);
			}
			query->pos = 0;
			if (block[i] == query->match[0]) {
				i--;
			}
			else {
				fwrite(block + i, sizeof(char), 1, outStream);
			}
			/*}*/
		}
	}
}

#include <string.h>

Query* NewQuery(char* match)
{
	Query* q = calloc(1, sizeof(Query));
	if (q == 0) return 0;
	q->match = match;
	q->pos = 0;
	q->size = strlen(match);
	q->pdata = getPrefixData(match, q->size);

	return q;
}

QueryPrefixData* getPrefixData(char* match, int len)
{
	QueryPrefixData* qpd = calloc(1, sizeof(QueryPrefixData));
	if (qpd == 0) return 0;

	int capacity = 10;
	qpd->positions = calloc(capacity, sizeof(int)); //todo vector
	qpd->lengths = calloc(capacity, sizeof(int));

	int t = 0;
	qpd->count = 0;

	for (int i = 1; i < len; i++) {
		if (match[t] == match[i]) {
			t++;
		}
		else {
			if (t > 0) {
				if (qpd->count > capacity) {
					capacity *= 2;
					qpd->positions = realloc(qpd->positions, sizeof(int) * capacity);
					qpd->lengths = realloc(qpd->lengths, sizeof(int) * capacity);
				}
				qpd->lengths[qpd->count] = t;
				qpd->positions[qpd->count] = i - t;
				qpd->count++;
			}
			t = 0;
		}
	}
	return qpd;
}


