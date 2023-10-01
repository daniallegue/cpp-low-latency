//Pointer aliasing ocurrs whenever a single memory location is accessible via more than one variable name
void func(int* a, int* b, int n) {
	for (int i = 0; i < n; i++) {
		a[i] = b;
		//Now b is accessible through b itself and all pointers in a
	}
}

void func2(int* __restrict a, int* __restrict b, int n) {
	for (int i = 0; i < n; i++) {
		a[i] = *b;
		//Now b is only accessible directly through b*
		//Now the cache or registers are not wasted
	}
}