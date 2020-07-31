#ifndef _UNI10_TEST_UNITENSOR_H_
#define _UNI10_TEST_UNITENSOR_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <vector>
#include <random>

#include "uni10.hpp"
#include "Generator.hpp"

using namespace uni10;
using namespace cytnx;

using namespace testing;
struct DenseUniTensorTest:testing::Test{
    UniTensor *T;
    UniTensor *T_t;
    Tensor *t;

    Bond bd1 = Bond(3);
    Bond bd2 = Bond(5);
    Bond bd3 = Bond(4);
    Bond bd1_t=Bond(4, BD_BRA);
    Bond bd2_t=Bond(3,BD_BRA);
    Bond bd3_t=Bond(5,BD_KET);
    Bond bd4_t=Bond(2, BD_KET);
    std::vector<Bond> bds = {bd1,bd2,bd3};
    std::vector<Bond> bds_t={bd1_t, bd2_t, bd3_t, bd4_t};
    void SetUp() override {
        T=new UniTensor(bds,{},1);
        T_t=new UniTensor(bds_t, {}, 2);
        t=new Tensor({3,5,4}, Type.Double, Device.cpu);
        t->fill(1.);
    }

    void TearDown() override{
        delete T;
        delete T_t;
        delete t;
    }


};


TEST_F(DenseUniTensorTest,Default_Costr_Test) {
    std::vector<uni10_int64> shape_val={3,5,4};

    auto shape=T->shape();
    ASSERT_EQ(T->dtype(),cytnx::Type.Double);
    ASSERT_EQ(T->device(),Device.cpu);
    ASSERT_EQ(T->uten_type(),UTenType.Dense);
    ASSERT_THAT(shape,ElementsAreArray(shape_val));
    ASSERT_THAT(T->bonds(),ElementsAreArray(bds));
    ASSERT_EQ(T->Rowrank(),1);
    ASSERT_THAT(T->labels(),ElementsAreArray({0,1,2}));
    ASSERT_FALSE(T->is_tag());
    ASSERT_FALSE(T->is_blockform());
    ASSERT_FALSE(T->is_braket_form());
    ASSERT_FALSE(T->is_diag());
    ASSERT_TRUE(T->is_contiguous());

}

TEST_F(DenseUniTensorTest,Clone_Test) {
    UniTensor T2=T->clone();


    ASSERT_EQ(T->dtype(),T2.dtype());
    ASSERT_EQ(T->device(),T2.device());
    ASSERT_THAT(T->shape(),ElementsAreArray(T2.shape()));
    ASSERT_THAT(T->bonds(),ElementsAreArray(T2.bonds()));
    ASSERT_EQ(T->Rowrank(),T2.Rowrank());
    ASSERT_THAT(T->labels(),ElementsAreArray(T2.labels()));
    ASSERT_EQ(T->is_tag(),T2.is_tag());
    ASSERT_EQ(T->is_blockform(), T2.is_blockform());
    ASSERT_EQ(T->is_braket_form(), T2.is_braket_form());
    ASSERT_EQ(T->is_diag(), T2.is_diag());
    ASSERT_EQ(T->is_contiguous(),T2.is_contiguous());
}


TEST_F(DenseUniTensorTest, Costr_from_Tensor_Test){
    UniTensor T1 = UniTensor(Tensor({2,3,4,5}),2);
    std::vector<Bond> bonds={Bond(2),Bond(3),Bond(4),Bond(5)};
    ASSERT_THAT(T1.shape(),ElementsAreArray({2,3,4,5}));
    ASSERT_EQ(T1.Rowrank(), 2);
    ASSERT_EQ(T1.dtype(),cytnx::Type.Double);
    ASSERT_EQ(T1.device(),Device.cpu);
    ASSERT_EQ(T1.uten_type(),UTenType.Dense);
    ASSERT_THAT(T1.bonds(),ElementsAreArray(bonds));
    ASSERT_THAT(T1.labels(),ElementsAreArray({0,1,2,3}));
    ASSERT_FALSE(T1.is_tag());
    ASSERT_FALSE(T1.is_blockform());
    ASSERT_FALSE(T1.is_braket_form());
    ASSERT_FALSE(T1.is_diag());
    ASSERT_TRUE(T1.is_contiguous());

}

TEST_F(DenseUniTensorTest, Costr_from_UniTensor_Test){
    UniTensor T1 = UniTensor(*T);

    ASSERT_THAT(T1.shape(),ElementsAreArray(T->shape()));
    ASSERT_EQ(T1.Rowrank(), T->Rowrank());
    ASSERT_EQ(T1.dtype(),T->dtype());
    ASSERT_EQ(T1.device(),T->device());
    ASSERT_EQ(T1.uten_type(),T->uten_type());
    ASSERT_THAT(T1.bonds(),ElementsAreArray(T->bonds()));
    ASSERT_THAT(T1.labels(),ElementsAreArray(T->labels()));


}

TEST_F(DenseUniTensorTest,Tagged_UniTensor_Test){

    ASSERT_THAT(T_t->shape(),ElementsAreArray({4,3,5,2}));
    ASSERT_THAT(T_t->bonds(),ElementsAreArray(bds_t));
    ASSERT_EQ(T_t->Rowrank(),2);
    ASSERT_THAT(T_t->labels(),ElementsAreArray({0,1,2,3}));
    ASSERT_FALSE(T_t->is_blockform());
    ASSERT_FALSE(T_t->is_braket_form());
    ASSERT_FALSE(T_t->is_diag());
    ASSERT_TRUE(T_t->is_contiguous());
    EXPECT_TRUE(T_t->is_tag());

}

TEST_F(DenseUniTensorTest, Mixed_Bonds_Fail_Test){
    std::vector<Bond> bonds={bd1,bd2,bd1_t, bd2_t};
    EXPECT_ANY_THROW(UniTensor T(bonds, {},2));

}

TEST_F(DenseUniTensorTest,Reshape_Test) {
    std::vector<uni10_int64> new_shape={3,5,4};

    UniTensor T2=T->reshape(new_shape,2);

    ASSERT_THAT(T2.shape(),ElementsAreArray(new_shape));
    ASSERT_THAT(T2.bonds(),ElementsAreArray(bds));
    ASSERT_EQ(T2.Rowrank(),2);
    ASSERT_THAT(T2.labels(),ElementsAreArray({0,1,2}));
    ASSERT_FALSE(T2.is_tag());
    ASSERT_FALSE(T2.is_blockform());
    ASSERT_FALSE(T2.is_braket_form());
    ASSERT_FALSE(T2.is_diag());
    ASSERT_TRUE(T2.is_contiguous());

}

TEST_F(DenseUniTensorTest,Reshape_inplace_Test) {
    std::vector<uni10_int64> new_shape={3,5,4};

    T->reshape_(new_shape,2);

    ASSERT_THAT(T->shape(),ElementsAreArray(new_shape));
    ASSERT_THAT(T->bonds(),ElementsAreArray(bds));
    ASSERT_EQ(T->Rowrank(),2);
    ASSERT_THAT(T->labels(),ElementsAreArray({0,1,2}));
    ASSERT_FALSE(T->is_tag());
    ASSERT_FALSE(T->is_blockform());
    ASSERT_FALSE(T->is_braket_form());
    ASSERT_FALSE(T->is_diag());
    ASSERT_TRUE(T->is_contiguous());

}

TEST_F(DenseUniTensorTest,Permute_Test){

    UniTensor T2=T->permute({1,2,0});
    UniTensor T3=T2.contiguous();
    ASSERT_THAT(T2.labels(),ElementsAreArray({1,2,0}));
    EXPECT_FALSE(T2.is_contiguous());
    EXPECT_TRUE(T3.is_contiguous());


}
TEST_F(DenseUniTensorTest,Permute_inplace_Test){

    T->permute_({1,2,0});
    ASSERT_THAT(T->labels(),ElementsAreArray({1,2,0}));
    EXPECT_FALSE(T->is_contiguous());
    T->contiguous_();
    EXPECT_TRUE(T->is_contiguous());


}

TEST_F(DenseUniTensorTest,At_Test){
    T->at<uni10_double>({1,2,3})=3.;
    EXPECT_DOUBLE_EQ(T->at<uni10_double>({1,2,3}), 3.);
}

TEST_F(DenseUniTensorTest,PutGet_Block_Test){


    Tensor A = arange(60,Type.Float).reshape({3,5,4});
    T->put_block(A);
    Tensor s=T->get_block();
    std::vector< uni10_uint64 > shape=s.shape();
    for(uni10_uint64 i=0;i<shape[0];i++){
        for (uni10_uint64 j=0;j<shape[1];j++){
            for (uni10_uint64 k=0;k<shape[2];k++){
                EXPECT_EQ(s.at<uni10_float>({i,j,k}),T->get_block().at<uni10_float>({i,j,k}) );
            }
        }
    }

 }
#endif
