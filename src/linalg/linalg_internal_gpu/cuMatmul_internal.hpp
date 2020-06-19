#ifndef __cuMatmul_internal_H__
#define __cuMatmul_internal_H__

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Storage.hpp"
#include "Type.hpp"

namespace cytnx{

    namespace linalg_internal{

        /// cuMatmul
        void cuMatmul_internal_cd(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_cf(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_d(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_f(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_i64(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_u64(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_i32(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_u32(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_u16(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_i16(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);
        void cuMatmul_internal_b(boost::intrusive_ptr<Storage_base> &out, const boost::intrusive_ptr<Storage_base> &inl, const boost::intrusive_ptr<Storage_base> &inr, const cytnx_int64 &Ml, const cytnx_int64 &Comm, const cytnx_int64 &Nr);

    }    
}



#endif
