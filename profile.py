# script to exercise the fastp2i library for PGO profile guided optimization

import fastp2i

for x in range(1000000):
    fastp2i.p2i([1,2,3,4,5,6,7,8,9,10], 5, [1,2,3,4,5,6,7,8,9,10])