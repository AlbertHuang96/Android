//
// Created by 86422 on 2024/3/12.
//

#ifndef RENDERPLAYGROUND_SETAFFINITY_H
#define RENDERPLAYGROUND_SETAFFINITY_H

int getMaxFreqIndex(int coreNum);
int getCoreNums();

int setAffinity(int core);

void testFalseSharing();

#endif //RENDERPLAYGROUND_SETAFFINITY_H