void performMatrixMultiplication(int processNumber) {
    // 배열 곱셈을 수행하는 작업을 여기에 구현
    int count = 0;
    int A[100][100], B[100][100];
    long result[100][100];

    // 모든 요소를 1000으로 초기화
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            A[i][j] = 10000000;
            B[i][j] = 20000000;
        }
    }

    while(count < 100){
        for(int k = 0; k < 100; k++){
            for(int i = 0; i < 100; i++){ 
                for(int j = 0; j < 100; j++){
                    result[k][j] += A[k][i] * B[i][j]; 
                }
            }
        } 
        count++;
    }
}
