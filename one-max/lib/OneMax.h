#pragma once

class OneMax {
    protected:
        bool *arr;
        bool *best;
        int size;
        int score;
        int bestScore;

    public:
        ~OneMax() { free(arr); free(best); }
        int getScore() { return score; }
        virtual void nextEnum() = 0;
        virtual void printArray() = 0;
        inline int getBestScore() { return bestScore; }
        inline bool done() { return bestScore == size; }
};