#ifndef _TESTNETWORK_H_
#define _TESTNETWORK_H_

#include "gtest/gtest.h"

#include <vector>
#include <random>

#include "uni10.hpp"

using namespace uni10;


TEST(Network,Launch_double){

  int d   = 4;
  int chi = 10;
  Bond bdi_d(BD_IN,d);
  Bond bdo_d(BD_OUT,d);
  Bond bdi_chi(BD_IN,chi);

  std::vector<Bond> bondsObs;
  bondsObs.push_back(bdi_d);
  bondsObs.push_back(bdi_d);
  bondsObs.push_back(bdo_d);
  bondsObs.push_back(bdo_d);

  std::vector<Bond> bondsAB;
  bondsAB.push_back(bdi_d);
  bondsAB.push_back(bdi_chi);
  bondsAB.push_back(bdi_chi);

  UniTensor<double> A(bondsAB);
  UniTensor<double> B(bondsAB);
  UniTensor<double> O(bondsObs);
  A.Randomize();
  B.Randomize();
  O.Randomize();
  UniTensor<double> AT(A), BT(B);

  UniTensor<double> T1;
  Network Net("testNet.net");
  Net.PutTensor(0, A);
  Net.PutTensor(1, B);
  Net.PutTensor(2, AT);
  Net.PutTensor(3, BT);
  Net.PutTensor(4, O);
  Net.Launch(T1);

  int labelA[]  = {0, 1, 2};
  int labelAT[] = {5, 6, 1};
  int labelB[]  = {3, 2, 4};
  int labelBT[] = {8, 4, 6};
  int labelO[]  = {0, 3, 5, 8};

  A.SetLabel(labelA); B.SetLabel(labelB);
  AT.SetLabel(labelAT); BT.SetLabel(labelBT);
  O.SetLabel(labelO);

  UniTensor<double> T2;
  ContractArgs(T2, A, AT, B, BT, O);

  double diff = T1[0] - T2[0];

  bool eq = fabs(diff) < 1E-10 ? true : false;


  EXPECT_EQ(true,eq);

}
TEST(Network,Launch_complex){

  int d   = 4;
  int chi = 10;
  Bond bdi_d(BD_IN,d);
  Bond bdo_d(BD_OUT,d);
  Bond bdi_chi(BD_IN,chi);

  std::vector<Bond> bondsObs;
  bondsObs.push_back(bdi_d);
  bondsObs.push_back(bdi_d);
  bondsObs.push_back(bdo_d);
  bondsObs.push_back(bdo_d);

  std::vector<Bond> bondsAB;
  bondsAB.push_back(bdi_d);
  bondsAB.push_back(bdi_chi);
  bondsAB.push_back(bdi_chi);

  UniTensor<std::complex<double> > A(bondsAB);
  UniTensor<std::complex<double> > B(bondsAB);
  UniTensor<std::complex<double> > O(bondsObs);
  A.Randomize();
  B.Randomize();
  O.Randomize();
  UniTensor<std::complex<double> > AT(A), BT(B);

  UniTensor<std::complex<double> > T1;
  Network Net("testNet.net");
  Net.PutTensor(0, A);
  Net.PutTensor(1, B);
  Net.PutTensor(2, AT);
  Net.PutTensor(3, BT);
  Net.PutTensor(4, O);
  Net.Launch(T1);

  int labelA[]  = {0, 1, 2};
  int labelAT[] = {5, 6, 1};
  int labelB[]  = {3, 2, 4};
  int labelBT[] = {8, 4, 6};
  int labelO[]  = {0, 3, 5, 8};

  A.SetLabel(labelA); B.SetLabel(labelB);
  AT.SetLabel(labelAT); BT.SetLabel(labelBT);
  O.SetLabel(labelO);

  UniTensor<std::complex<double> > T2;
  ContractArgs(T2, A, AT, B, BT, O);

  std::complex<double> diff = T1[0] - T2[0];

  bool eq = std::norm(diff) < 1E-10 ? true : false;

  EXPECT_EQ(true,eq);

}
TEST(Network,Launch_mix){

  int d   = 4;
  int chi = 10;
  Bond bdi_d(BD_IN,d);
  Bond bdo_d(BD_OUT,d);
  Bond bdi_chi(BD_IN,chi);

  std::vector<Bond> bondsObs;
  bondsObs.push_back(bdi_d);
  bondsObs.push_back(bdi_d);
  bondsObs.push_back(bdo_d);
  bondsObs.push_back(bdo_d);

  std::vector<Bond> bondsAB;
  bondsAB.push_back(bdi_d);
  bondsAB.push_back(bdi_chi);
  bondsAB.push_back(bdi_chi);

  UniTensor<std::complex<double> > A(bondsAB);
  UniTensor<std::complex<double> > B(bondsAB);
  UniTensor<double> O(bondsObs);
  A.Randomize();
  B.Randomize();
  O.Randomize();
  UniTensor<std::complex<double> > AT(A), BT(B);

  UniTensor<std::complex<double> > T1;
  Network Net("testNet.net");
  Net.PutTensor(0, A);
  Net.PutTensor(1, B);
  Net.PutTensor(2, AT);
  Net.PutTensor(3, BT);
  Net.PutTensor(4, O);
  Net.Launch(T1);

  int labelA[]  = {0, 1, 2};
  int labelAT[] = {5, 6, 1};
  int labelB[]  = {3, 2, 4};
  int labelBT[] = {8, 4, 6};
  int labelO[]  = {0, 3, 5, 8};

  A.SetLabel(labelA); B.SetLabel(labelB);
  AT.SetLabel(labelAT); BT.SetLabel(labelBT);
  O.SetLabel(labelO);

  UniTensor<std::complex<double> > T2;
  ContractArgs(T2, A, AT, B, BT, O);

  std::complex<double> diff = T1[0] - T2[0];

  bool eq = std::norm(diff) < 1E-10 ? true : false;

  EXPECT_EQ(true,eq);

}
#endif
