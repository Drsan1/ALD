/** \file TQueue.c
 *  \brief Implementace API pro typ fronta (realizace pomocí lineárního jednosměrně vázaného seznamu)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TQueue.c 3215 2026-02-20 16:26:16Z petyovsky $
 */

#include <assert.h>
#include "TQueue.h"

/** \brief Úplná definice privátního typu QueueNode
 *  \details Privátní typ QueueNode (nedostupný mimo funkce ze souboru TQueue.c) reprezentuje typ pro jednotlivé uzly lineárního jednosměrně vázaného seznamu.
 */
struct TQueueNode
	{
	struct TQueueNode *iNext;				///< Ukazatel na následující uzel lineárního jednosměrně vázaného seznamu
	TQueueElement iValue;					///< Element fronty uložený v uzlu lineárního jednosměrně vázaného seznamu
	};

[[nodiscard]] static inline bool queue_invariant(const struct TQueue *aQueue)
	{
	return (aQueue->iFront && aQueue->iBack) || (!aQueue->iFront && !aQueue->iBack);
	}

void queue_init(struct TQueue *aQueue)
	{
	if (!aQueue)
		return;
	*aQueue = (struct TQueue) { .iFront = NULL, .iBack = NULL}; //Edit

	assert(queue_invariant(aQueue));
	}

bool queue_is_empty(const struct TQueue *aQueue)
	{
	if (!aQueue)
		return true;
	assert(queue_invariant(aQueue));
	return !aQueue->iFront; //je jedno jestli back nebo front invariant to zaridi
	}

bool /* TQueueIterator */ queue_front(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	if (!aValue || queue_is_empty(aQueue))
		return false;
	assert(queue_invariant(aQueue));
	*aValue = aQueue->iFront->iValue;
	return true;
	}

bool /* TQueueIterator */ queue_back(const struct TQueue *aQueue, TQueueElement *aValue)
	{
	if (!aValue || queue_is_empty(aQueue))
		return false;
	assert(queue_invariant(aQueue));
	*aValue = aQueue->iBack->iValue;
	return true;
	}

bool queue_push(struct TQueue *aQueue, TQueueElement aValue)
	{
	if (!aQueue)
		return false;
	assert(queue_invariant(aQueue));
	struct TQueueNode* ptr = malloc(sizeof(struct TQueueNode));
	if (!ptr)
		return false;
	*ptr = (struct TQueueNode){ .iNext = NULL, .iValue = aValue};

	if (queue_is_empty(aQueue))
		aQueue->iFront = ptr;
	else
		aQueue->iBack->iNext = ptr;
	aQueue->iBack = ptr;
	assert(queue_invariant(aQueue));
	return true;
	}

bool queue_pop(struct TQueue *aQueue)
	{
	if (queue_is_empty(aQueue))
		return false;
	assert(queue_invariant(aQueue));
	struct TQueueNode* ptr = aQueue->iFront;
	aQueue->iFront = aQueue->iFront->iNext;
	if (!aQueue->iFront)
		aQueue->iBack = NULL;
	free(ptr);
	assert(queue_invariant(aQueue));
	return true;
	}

void queue_destroy(struct TQueue *aQueue)
	{
	while (!queue_is_empty(aQueue))
		queue_pop(aQueue);
	}

struct TQueueIterator queue_iterator_begin(const struct TQueue *aQueue)
	{
	if(aQueue)
		if(!queue_is_empty(aQueue)) //aQueue.iFront
			return (struct TQueueIterator) { .iQueue = aQueue, .iActual = aQueue->iFront };
	return (struct TQueueIterator) { .iQueue = NULL, .iActual = NULL };
	}

bool queue_iterator_is_valid(const struct TQueueIterator *aIter)
	{
	if (aIter) //pokud je platny jde dal
		if (aIter->iQueue)
			if (aIter->iQueue->iFront)
				return aIter->iActual; // != NULL
	return false;
	}

bool queue_iterator_to_next(struct TQueueIterator *aIter)
	{
	if(aIter){
		if (queue_iterator_is_valid(aIter)) {
			aIter->iActual = aIter->iActual->iNext;
			return aIter->iActual;
		}
		*aIter = (struct TQueueIterator) { .iQueue = NULL, .iActual = NULL };
	}
	return false;
	}

TQueueElement queue_iterator_value(const struct TQueueIterator *aIter)
	{
	if (queue_iterator_is_valid(aIter))
		return aIter->iActual->iValue;
	return (TQueueElement) { 0 };
	}

bool queue_iterator_set_value(const struct TQueueIterator *aIter, TQueueElement aValue)
	{
	if (!queue_iterator_is_valid(aIter))
		return false;
	aIter->iActual->iValue = aValue;
	return true;
	}
