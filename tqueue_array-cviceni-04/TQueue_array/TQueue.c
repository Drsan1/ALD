/** \file TQueue.c
 *  \brief Implementace API pro typ fronta (realizace pomocí kruhového pole)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TQueue.c 3215 2026-02-20 16:26:16Z petyovsky $
 */

#include <assert.h>
#include "TQueue.h"

[[nodiscard]] static inline bool queue_invariant(const struct TQueue *aQueue)
	{
	return aQueue->iBackPos < QUEUE_MAXCOUNT && aQueue->iFrontPos < QUEUE_MAXCOUNT;
	}

void queue_init(struct TQueue *aQueue)
	{
	if (!aQueue)
		return;
	/*aQueue->iFrontPos = 0;
	aQueue->iBackPos = 0;*/
	*aQueue = (struct TQueue) { .iBackPos = 0, .iFrontPos = 0 };
	assert(queue_invariant(aQueue));
	}

bool queue_is_empty(const struct TQueue *aQueue)
	{
	if (!aQueue)
		return true;
	assert(queue_invariant(aQueue));
	return aQueue->iFrontPos == aQueue->iBackPos;
	}

bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	if (queue_is_empty(aQueue) || !aValue)
		return false;
	assert(queue_invariant(aQueue));
	*aValue = aQueue->iValues[aQueue->iFrontPos];
	assert(queue_invariant(aQueue));
	return true;
	}

bool /* TQueueIterator */ queue_back(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	if (queue_is_empty(aQueue) || !aValue)
		return false;
	size_t pos = (aQueue->iBackPos) ? (aQueue->iBackPos - 1) : (QUEUE_MAXCOUNT - 1); //true jestli backpos je vesti jak 0
	assert(queue_invariant(aQueue));
	*aValue = aQueue->iValues[pos];
	assert(queue_invariant(aQueue));
	return true;
	}

bool queue_push(struct TQueue *aQueue, TQueueElement aValue)
	{
	if (!aQueue)
		return false;
	size_t nextpos = (aQueue->iBackPos + 1) % QUEUE_MAXCOUNT;
	if (nextpos == aQueue->iFrontPos)
		return false; //plna fronta
	assert(queue_invariant(aQueue));
	aQueue->iValues[aQueue->iBackPos] = aValue;
	aQueue->iBackPos = nextpos;
	assert(queue_invariant(aQueue));
	return true;
	}

bool queue_pop(struct TQueue *aQueue)
	{
	if (queue_is_empty(aQueue))
		return false;
	assert(queue_invariant(aQueue));
	aQueue->iFrontPos = (aQueue->iFrontPos + 1) % QUEUE_MAXCOUNT;
	assert(queue_invariant(aQueue));
	return true;
	}

void queue_destroy(struct TQueue *aQueue)
	{
	if (!aQueue)
		return;
	assert(queue_invariant(aQueue));
	aQueue->iFrontPos = 0;
	aQueue->iBackPos = 0;
	assert(queue_invariant(aQueue));
	}

struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue)
	{
	if(!queue_is_empty(aQueue))
		return (struct TQueueIterator) { .iQueue = aQueue, .iPos = aQueue->iFrontPos };
	return (struct TQueueIterator) { .iQueue = NULL, .iPos = 0 };
	}

bool queue_iterator_is_valid(const struct TQueueIterator *aIter)
	{
	if (!aIter || !aIter->iQueue)
		return false;
	if (queue_is_empty(aIter->iQueue))
		return false;
	return aIter->iPos != aIter->iQueue->iBackPos;
	}

bool queue_iterator_to_next(struct TQueueIterator *aIter)
	{
	if (queue_iterator_is_valid(aIter)) {
		aIter->iPos = (aIter->iPos + 1) % QUEUE_MAXCOUNT;
		if (aIter->iPos != aIter->iQueue->iBackPos)
			return true;
	}

	return false;
	}

TQueueElement queue_iterator_value(const struct TQueueIterator *aIter)
	{
	if (queue_iterator_is_valid(aIter))
		return aIter->iQueue->iValues[aIter->iPos];
	return (TQueueElement) { 0 };
	}

bool queue_iterator_set_value(const struct TQueueIterator *aIter, TQueueElement aValue)
	{
	if (queue_iterator_is_valid(aIter)) {
		//struct TQueue* queue = aIter->iQueue;
		//queue->iValues[aIter->iPos] = aValue;
		((struct TQueue*)aIter->iQueue)->iValues[aIter->iPos] = aValue;
		return true;
	}

	return false;
	}
