#include "testBond.h"
#include "testUniTensor.h"
#include "testSymmetry.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

}
