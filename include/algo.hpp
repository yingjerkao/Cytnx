#ifndef _algo_H_
#define _algo_H_

#include "Type.hpp"
#include "cytnx_error.hpp"
#include "Tensor.hpp"
#include "Storage.hpp"
#include "Scalar.hpp"


namespace cytnx{
    namespace algo{
        

        //Sort:
        //=================================================
        /**
        @brief sort Tensor along last axis. 
        @param Tin Tensor #1
        @return 
            [Tensor] 

        */
        Tensor Sort(const Tensor &Tin);

    }//algo
}//cytnx


#endif
