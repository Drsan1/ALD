#include <stdio.h>
#include <stdlib.h>
#include "TVector.h"
#include "check.h"

#define MAX 1000

int main(int argc, char* argv[]) {
	TVectorElement buffer[MAX];
	size_t count = 0;
	float num;

	while (scanf(TVECTOR_ELEMENT_FRMSTR, &num) == 1) {
		if (num < 0)
			break;
		if (count < MAX) {
			buffer[count] = num;
			count++;
		}
	}

	struct TVector tvector1;

	if (!vector_init(&tvector1, count))
		return 1;

	for (size_t i = 0; i < count; i++) {
		tvector1.iValues[i] = buffer[i];
		if (printf(TVECTOR_ELEMENT_FRMSTR " ", tvector1.iValues[i]) < 0)
			return 2;
	}

	float value;
	putchar('\n');
	if (scanf("%f", &value) != 1)
		return 3;

	struct TVector tvector2;
	struct TVector tvector3;

	if (!split(&tvector1, value, &tvector2, &tvector3))
		return 4;
	
	for (size_t i = 0; i < vector_size(&tvector2); i++) {
		if (printf(TVECTOR_ELEMENT_FRMSTR " ", tvector2.iValues[i]) < 0)
			return 5;
	}
	vector_destroy(&tvector2);

	putchar('\n');

	for (size_t i = 0; i < vector_size(&tvector3); i++) {
		if (printf(TVECTOR_ELEMENT_FRMSTR " ", tvector3.iValues[i]) < 0)
			return 6;
	}
	vector_destroy(&tvector3);

	vector_destroy(&tvector1);
	return 0;
}

