
#include <gtest/gtest.h>
#include <glog/logging.h>

#include "src/user/normal_user.h"

TEST(User, Name) {
    wechat::User* user = new wechat::NormalUser("test_name");
    EXPECT_EQ(user->GetUserName(), "test_name");
}

TEST(User, WrongName) {
    wechat::User* user = new wechat::NormalUser("test_name");
    EXPECT_NE(user->GetUserName(), "test_wrone_ser");
}


int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
