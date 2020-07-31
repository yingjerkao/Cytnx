#ifndef __UNI10_TEST_SYMMETRY_H__
#define __UNI10_TEST_SYMMETRY_H__

#include <time.h>

#include <iostream>
#include <vector>
#include <map>

#include "gtest/gtest.h"

#include "uni10.hpp"

using namespace uni10;


TEST(SymmetryTest,Default_Costr_Test) {
    Symmetry sym;
    ASSERT_EQ(sym.stype(),SymType.U);
    ASSERT_EQ(sym.n(),1);
}

TEST(SymmetryTest,U1_Costr_Test) {
    Symmetry sym=Symmetry::U1();
    ASSERT_EQ(sym.stype(),SymType.U);
    ASSERT_EQ(sym.n(),1);
}


TEST(SymmetryTest,Zn_Costr_Test) {
    Symmetry sym=Symmetry::Zn(3);
    ASSERT_EQ(sym.stype(),SymType.Z);
    ASSERT_EQ(sym.n(),3);
}

TEST(SymmetryTest,check_qnum_Zn_Test) {
    Symmetry sym=Symmetry::Zn(3);
    ASSERT_TRUE(sym.check_qnums({0,1,2}));
    ASSERT_FALSE(sym.check_qnum(3));
}
TEST(SymmetryTest,check_qnum_U1_Test) {
    Symmetry sym=Symmetry::U1();
    ASSERT_TRUE(sym.check_qnums({0,1,2}));
    ASSERT_TRUE(sym.check_qnum(100));
}
TEST(SymmetryTest,combine_rule_Zn_Test) {
    Symmetry sym=Symmetry::Zn(3);
    std::vector<uni10_int64> qnums1={0,1,2};
    std::vector<uni10_int64> qnums2={0,1,2};
    auto qnums3=sym.combine_rule(qnums1,qnums2);
    std::vector<uni10_int64> qnums_vals={0,1,2,1,2,0,2,0,1};
    for (auto i=0;i<qnums3.size();i++){
        ASSERT_EQ(qnums3[i],qnums_vals[i]);
    }

}

TEST(SymmetryTest,combine_rule_Zn_inplace_Test) {
    Symmetry sym=Symmetry::Zn(3);
    std::vector<uni10_int64> qnums1={0,1,2};
    std::vector<uni10_int64> qnums2={0,1,2};
    std::vector<uni10_int64> qnums3;
    sym.combine_rule_(qnums3, qnums1,qnums2);
    std::vector<uni10_int64> qnums_vals={0,1,2,1,2,0,2,0,1};
    for (auto i=0;i<qnums3.size();i++){
        ASSERT_EQ(qnums3[i],qnums_vals[i]);
    }

}

TEST(SymmetryTest,combine_rule_U1_Test) {
    Symmetry sym=Symmetry::U1();
    std::vector<uni10_int64> qnums1={0,1,2};
    std::vector<uni10_int64> qnums2={-2,0,2};
    auto qnums3=sym.combine_rule(qnums1,qnums2);
    std::vector<uni10_int64> qnums_vals={-2,0,2,-1,1,3,0,2,4};
    for (auto i=0;i<qnums3.size();i++){
        ASSERT_EQ(qnums3[i],qnums_vals[i]);
    }

}

TEST(SymmetryTest,combine_rule_U1_inplace_Test) {
    Symmetry sym=Symmetry::U1();
    std::vector<uni10_int64> qnums1={0,1,2};
    std::vector<uni10_int64> qnums2={-2,0,2};
    std::vector<uni10_int64> qnums3;
    sym.combine_rule_(qnums3, qnums1,qnums2);
    std::vector<uni10_int64> qnums_vals={-2,0,2,-1,1,3,0,2,4};
    for (auto i=0;i<qnums3.size();i++){
        ASSERT_EQ(qnums3[i],qnums_vals[i]);
    }

}

TEST(SymmetryTest,clone_U1_Test) {
    Symmetry sym1=Symmetry::U1();
    auto sym2=sym1.clone();
    ASSERT_EQ(sym1,sym2);

}

TEST(SymmetryTest,clone_Zn_Test) {
    Symmetry sym1=Symmetry::Zn(5);
    auto sym2=sym1.clone();
    ASSERT_EQ(sym1,sym2);

}
#endif
