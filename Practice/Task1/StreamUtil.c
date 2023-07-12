#include "StreamUtil.h"
#include <stdlib.h>

#include "core/CommonTypes.h"

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
			if (query->pos > 0)   //провал при совпадении 
			{ 
				int prefixOffset = GetInteger(
					mapGetByInteger(query->prefixMap, query->pos)
				);
				if (prefixOffset > 0) // с этого места может начаться query
				{
					fwrite(query, sizeof(char), query->pos - prefixOffset, outStream);
					query->pos = prefixOffset;
					continue;
				}
				else if (block[i] == query->match[0]) // с этого символа может начаться query
				{
					i--;
					fwrite(query, sizeof(char), query->pos - 1, outStream);
					query->pos = 0;
					continue;
				}
				else {
					fwrite(query, sizeof(char), query->pos, outStream);
				}
			}
			query->pos = 0;
			fwrite(block + i, sizeof(char), 1, outStream);
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
	getPrefixData(q, match, q->size);

	return q;
}

void getPrefixData(Query* q, char* match, int len)
{

	q->prefixMap = NewBinaryTree();

	int t = 0;

	for (int i = 1; i < len + 1; i++)
	{
		if (match[t] == match[i])
		{
			t++;
		}
		else
		{ 
			if (t > 0) {
				mapEmplace(q->prefixMap, NewInteger(i), NewInteger(t));
			}
			t = 0;
		}
	}
}


