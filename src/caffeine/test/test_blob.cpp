#include <cstring>
#include <cuda_runtime.h>

#include "gtest/gtest.h"
#include "caffeine/common.hpp"
#include "caffeine/blob.hpp"
#include "caffeine/filler.hpp"

#include "caffeine/test/test_caffeine_main.hpp"

namespace caffeine {
  
template <typename Dtype>
class BlobSimpleTest : public ::testing::Test {
 protected:
  BlobSimpleTest()
      : blob_(new Blob<Dtype>()),
        blob_preshaped_(new Blob<Dtype>(2, 3, 4, 5)) {};
  virtual ~BlobSimpleTest() { delete blob_; delete blob_preshaped_; }
  Blob<Dtype>* const blob_;
  Blob<Dtype>* const blob_preshaped_;
};

typedef ::testing::Types<float, double> Dtypes;
TYPED_TEST_CASE(BlobSimpleTest, Dtypes);

TYPED_TEST(BlobSimpleTest, TestInitialization) {
  EXPECT_TRUE(this->blob_);
  EXPECT_TRUE(this->blob_preshaped_);
  EXPECT_EQ(this->blob_preshaped_->num(), 2);
  EXPECT_EQ(this->blob_preshaped_->channels(), 3);
  EXPECT_EQ(this->blob_preshaped_->height(), 4);
  EXPECT_EQ(this->blob_preshaped_->width(), 5);
  EXPECT_EQ(this->blob_preshaped_->count(), 120);
  EXPECT_EQ(this->blob_->num(), 0);
  EXPECT_EQ(this->blob_->channels(), 0);
  EXPECT_EQ(this->blob_->height(), 0);
  EXPECT_EQ(this->blob_->width(), 0);
  EXPECT_EQ(this->blob_->count(), 0);
}

TYPED_TEST(BlobSimpleTest, TestPointers) {
  EXPECT_TRUE(this->blob_preshaped_->gpu_data());
  EXPECT_TRUE(this->blob_preshaped_->cpu_data());
  EXPECT_TRUE(this->blob_preshaped_->mutable_gpu_data());
  EXPECT_TRUE(this->blob_preshaped_->mutable_cpu_data());
}

TYPED_TEST(BlobSimpleTest, TestReshape) {
  this->blob_->Reshape(2, 3, 4, 5);
  EXPECT_EQ(this->blob_->num(), 2);
  EXPECT_EQ(this->blob_->channels(), 3);
  EXPECT_EQ(this->blob_->height(), 4);
  EXPECT_EQ(this->blob_->width(), 5);
  EXPECT_EQ(this->blob_->count(), 120);
}

TYPED_TEST(BlobSimpleTest, TestCopyConstructor) {
  Blob<TypeParam> source(2, 3, 4, 5);
  FillerParameter filler_param;
  UniformFiller<TypeParam> filler(filler_param);
  filler.Fill(&source);
  Blob<TypeParam> target(source);
  const TypeParam* source_data = source.cpu_data();
  const TypeParam* target_data = target.cpu_data();
  EXPECT_EQ(target.num(), source.num());
  EXPECT_EQ(target.channels(), source.channels());
  EXPECT_EQ(target.height(), source.height());
  EXPECT_EQ(target.width(), source.width());
  EXPECT_EQ(target.count(), source.count());
  for (int i = 0; i < source.count(); ++i) {
    EXPECT_EQ(source_data[i], target_data[i]);
  }
}

}
