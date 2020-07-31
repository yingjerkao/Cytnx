#ifndef __UNI10_TEST_BOND_H__
#define __UNI10_TEST_BOND_H__

#include <ctime>

#include <iostream>
#include <vector>
#include <map>

#include "gtest/gtest.h"
#include "uni10.hpp"

using namespace uni10;


TEST(BondTest,Default_Costr_Test){
    // brief Default constructor
    Bond bond1;

    ASSERT_EQ( bond1.type(), bondType::BD_REG);
    ASSERT_EQ(bond1.dim(),0 );
    ASSERT_EQ(bond1.Nsym(),0);

}

TEST(BondTest, CopyCostr_Test){
    Bond bond1;
    Bond bond2=bond1;
    ASSERT_EQ(bond1.type(), bond2.type());
    ASSERT_EQ(bond1.dim(), bond2.dim());
    ASSERT_EQ(bond1.Nsym(),0);
}

TEST(BondTest, ConstrByDim_Test){
    Bond bond(10);
    ASSERT_EQ( bond.dim(),10);
    ASSERT_EQ(bond.type(),BD_REG);
}

TEST(BondTest,Tagged_Bond_Test) {
    Bond bond(10, BD_KET);
    ASSERT_EQ( bond.dim(),10);
    ASSERT_EQ(bond.type(),BD_KET);
}

TEST(BondTest,U1_Bond_Test){
    Bond bd_dqu1 = Bond(3, BD_BRA,{{0,1},{2,2},{3,4}});
    const std::vector< std::vector<uni10_int64 > >  qnums_val{{0,1},{2,2},{3,4}};
    auto qnums=bd_dqu1.qnums();
    for (auto i=0;i<qnums.size();  ++i){
        for (auto j=0; j<qnums[i].size(); ++j){
            ASSERT_EQ(qnums_val[i][j],qnums[i][j]);
        }

    }
    ASSERT_EQ(bd_dqu1.Nsym(),2);
}
TEST(BondTest, Zn_Bond_Test){
    Bond bond=Bond(1,BD_KET, {{1,2}},{Symmetry::Zn(2),Symmetry::Zn(3)});
    ASSERT_EQ(bond.dim(),1);
    ASSERT_EQ(bond.type(),BD_KET);
    ASSERT_TRUE(bond.syms()[0]==Symmetry::Zn(2));
    ASSERT_TRUE(bond.syms()[1]==Symmetry::Zn(3));
    ASSERT_EQ(bond.Nsym(),2);
}


TEST(BondTest,CombineBond_U1_Test){
    Bond bd_dqu1 = Bond(1, BD_BRA,{{0,1}});
    Bond bd_dqu2 = Bond(1, BD_BRA,{{0,2}});
    Bond bd_dqu3 = bd_dqu1.combineBond(bd_dqu2);
    auto qnums1=bd_dqu1.qnums();
    auto qnums2=bd_dqu2.qnums();
    auto qnums3=bd_dqu3.qnums();
    const std::vector< std::vector<uni10_int64 > >  qnums_val={{0,3}};
    ASSERT_EQ(bd_dqu3.dim(),1);
    ASSERT_EQ(bd_dqu3.type(),bondType::BD_BRA);

    ASSERT_EQ(qnums3.size(), 1);
    for (auto i=0;i<qnums3.size();  ++i) {
        for (auto j = 0; j < qnums3[i].size(); ++j) {
            ASSERT_EQ(qnums_val[i][j], qnums3[i][j]);
        }
    }

    }

TEST(BondTest,combineBond_inplace_U1_Test){
    Bond bd_dqu1 = Bond(1, BD_BRA,{{0,1}});
    Bond bd_dqu2 = Bond(1, BD_BRA,{{0,2}});
    bd_dqu1.combineBond_(bd_dqu2);
    auto qnums1=bd_dqu1.qnums();

    const std::vector< std::vector<uni10_int64 > >  qnums_val={{0,3}};
    ASSERT_EQ(bd_dqu1.dim(),1);
    ASSERT_EQ(bd_dqu1.type(),bondType::BD_BRA);

    ASSERT_EQ(qnums1.size(), 1);
    for (auto i=0;i<qnums1.size();  ++i) {
        for (auto j = 0; j < qnums1[i].size(); ++j) {
            ASSERT_EQ(qnums_val[i][j], qnums1[i][j]);
        }
    }

}

TEST(BondTest,combineBond_MultiU1_Test){
    Bond bd_dqu1 = Bond(2, BD_BRA,{{0,1},{2,2}});
    Bond bd_dqu2 = Bond(1, BD_BRA,{{0,-1}});
    Bond bd_dqu3 = bd_dqu1.combineBond(bd_dqu2);
    auto qnums1=bd_dqu1.qnums();
    auto qnums2=bd_dqu2.qnums();
    auto qnums3=bd_dqu3.qnums();
    const std::vector< std::vector<uni10_int64 > >  qnums_val={{0,0},{2,1}};
    ASSERT_EQ(bd_dqu3.dim(),2);
    ASSERT_EQ(bd_dqu3.type(),bondType::BD_BRA);

    ASSERT_EQ(qnums3.size(), 2);
    for (auto i=0;i<qnums3.size();  ++i) {
        for (auto j = 0; j < qnums3[i].size(); ++j) {
            ASSERT_EQ(qnums_val[i][j], qnums3[i][j]);
        }
    }

}

TEST(BondTest,CombineBond_MultiU1_inplace_Test){
    Bond bd_dqu1 = Bond(2, BD_BRA,{{0,1},{2,2}});
    Bond bd_dqu2 = Bond(1, BD_BRA,{{0,-1}});
    bd_dqu1.combineBond_(bd_dqu2);
    auto qnums1=bd_dqu1.qnums();
    const std::vector< std::vector<uni10_int64 > >  qnums_val={{0,0},{2,1}};
    ASSERT_EQ(bd_dqu1.dim(),2);
    ASSERT_EQ(bd_dqu1.type(),bondType::BD_BRA);


    ASSERT_EQ(qnums1.size(), 2);
    for (auto i=0;i<qnums1.size();  ++i) {
        for (auto j = 0; j < qnums1[i].size(); ++j) {
            ASSERT_EQ(qnums_val[i][j], qnums1[i][j]);
        }
    }

}
TEST(BondTest,CombineBond_MulSymm_Test){
    Bond bond1 = Bond(2, BD_BRA,{{0,1},{1,3}},{{Symmetry::Zn(2),Symmetry::U1()}} );
    Bond bond2 = Bond(1, BD_BRA,{{0,-1}}, {{Symmetry::Zn(2),Symmetry::U1()}});
    Bond bond3 = bond1.combineBond(bond2);
    auto qnums1=bond1.qnums();
    auto qnums2=bond2.qnums();
    auto qnums3=bond3.qnums();
    const std::vector< std::vector<uni10_int64 > >  qnums_val={{0,0},{1,2}};
    ASSERT_EQ(bond3.dim(),2);
    ASSERT_EQ(bond3.type(),bondType::BD_BRA);
    ASSERT_EQ(bond3.Nsym(),2);
    ASSERT_EQ(qnums3.size(), 2);
    for (auto i=0;i<qnums3.size();  ++i) {
        for (auto j = 0; j < qnums3[i].size(); ++j) {
            ASSERT_EQ(qnums_val[i][j], qnums3[i][j]);
        }
    }

}
TEST(BondTest, SetType_TEST){
    Bond bond(10);
    ASSERT_EQ( bond.dim(),10);
    ASSERT_EQ(bond.type(),BD_REG);
    bond.set_type(BD_BRA);
    ASSERT_EQ(bond.type(),BD_BRA);
    ASSERT_EQ( bond.dim(),10);
}

TEST(BondTest, ClearType_TEST){
    Bond bond(5, BD_BRA);
    ASSERT_EQ( bond.dim(),5);
    ASSERT_EQ(bond.type(),BD_BRA);
    bond.clear_type();
    ASSERT_EQ(bond.type(),BD_REG);
    ASSERT_EQ( bond.dim(),5);
}

TEST(BondTest, Clone_TEST){
    Bond bond1 = Bond(2, BD_BRA,{{0,1},{1,3}},{{Symmetry::Zn(2),Symmetry::U1()}} );
    Bond bond2=bond1.clone();
    auto qnums1=bond1.qnums();
    auto qnums2=bond2.qnums();
    auto sym1=bond1.syms();
    auto sym2=bond2.syms();

    ASSERT_EQ( bond1.dim(),bond2.dim());
    ASSERT_EQ(bond1.type(),bond2.type());
    for (auto i=0;i<qnums1.size();  ++i) {
        for (auto j = 0; j < qnums1[i].size(); ++j) {
            ASSERT_EQ(qnums1[i][j], qnums2[i][j]);
        }
    }
    for (auto i=0;i<sym1.size();  ++i) {
            ASSERT_EQ(sym1[i], sym2[i]);
    }
}

TEST(BondTest, SymsClone_TEST){
    Bond bond1 = Bond(2, BD_BRA,{{0,1},{1,3}},{{Symmetry::Zn(2),Symmetry::U1()}} );
    Bond bond2=Bond(3,BD_KET, {{0,1},{1,3},{0,4}}, bond1.syms_clone());

    auto sym1=bond1.syms();
    auto sym2=bond2.syms();

    for (auto i=0;i<sym1.size();  ++i) {
        ASSERT_EQ(sym1[i], sym2[i]);
    }
}

TEST(BondTest,GetDegeneracy_TEST){
    Bond bond = Bond(6, BD_KET,{{0,1},{2,2},{3,4},{-2,-4},{-1,-2},{3,4}});
    ASSERT_EQ(bond.getDegeneracy({3,4}),2);
    ASSERT_EQ(bond.getDegeneracy({-1,-2}),1);
}

TEST(BondTest,GetUniqueQnum_TEST){
    Bond bond = Bond(6, BD_KET,{{0,1},{2,2},{3,4},{-2,-4},{-1,-2},{3,4}});
    const std::vector< std::vector<uni10_int64 > > qnums_val={{-2,-4},{-1,-2},{0,1},{2,2},{3,4}};
    auto qnums=bond.getUniqueQnums();
    for (auto i=0; i<qnums.size();i++){
        for (auto j=0;j<qnums[i].size();j++){
            ASSERT_EQ(qnums[i][j],qnums_val[i][j]);
        }
    }
}

TEST(BondTest,GetUniqueQnums_counts_TEST){
    Bond bond = Bond(6, BD_KET,{{0,1},{2,2},{3,4},{-2,-4},{-1,-2},{3,4}});
    std::vector<uni10_uint64> counts;
    const std::vector< std::vector<uni10_int64 > > qnums_val={{-2,-4},{-1,-2},{0,1},{2,2},{3,4}};
    const std::vector<uni10_uint64 > counts_val={1,1,1,1,2};
    auto qnums=bond.getUniqueQnums(counts);
    for (auto i=0; i<qnums.size();i++){
        for (auto j=0;j<qnums[i].size();j++){
            ASSERT_EQ(qnums[i][j],qnums_val[i][j]);
        }
        ASSERT_EQ(counts[i],counts_val[i]);
    }
}
#endif
