/*
 * Normaliz 2.7
 * Copyright (C) 2012 Christof Soeger
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* 
 * This header provide some dummy replacements of OpenMP functions. We use it
 * to compile Normaliz without OpenMP.
 */

#ifndef MY_OMP_H_
#define MY_OMP_H_

#ifdef _OPENMP
#include <omp.h>
#else

int omp_get_level(){
    return 0;
}

int omp_get_active_level() {
    return 0;
}

int omp_get_thread_num() {
    return 0;
}

int omp_get_max_threads() {
    return 1;
}

int omp_get_ancestor_thread_num(int level) {
    return 0;
}

#endif /* ifndef _OPENMP */
#endif /* MY_OMP_H_ */
