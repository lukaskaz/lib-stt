#include "mocks/mock_helpers.hpp"
#include "mocks/mock_shell.hpp"
#include "stt/interfaces/googleapi.hpp"
#include "stt/interfaces/v1/googlecloud.hpp"
#include "stt/interfaces/v2/googlecloud.hpp"

using namespace testing;

class TestStt : public Test
{
  public:
    std::shared_ptr<NiceMock<ShellMock>> shellMock =
        std::make_shared<NiceMock<ShellMock>>();
    std::shared_ptr<NiceMock<HelpersMock>> helpersMock =
        std::make_shared<NiceMock<HelpersMock>>();

  protected:
    void SetUp() override
    {}

    void TearDown() override
    {}
};

TEST_F(TestStt, googleApiValidTextShellCmdAndHelpersCalled_ResultValid)
{
    std::string initialtext = "przykładowy tekst rozpoznany z głosu";
    double initialconfid = 0.825;
    auto initialquality = (uint32_t)std::lround(100 * initialconfid);
    std::string resultjson = {
        "{\"result\":[{\"alternative\":[{\"transcript\":\"" + initialtext +
        "\","
        "\"confidence\":" +
        std::to_string(initialconfid) +
        "}],\"final\":true}],\"result_index\":0}"};
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    ON_CALL(*helpersMock, uploadFile(HasSubstr("lang=pl-PL"), _, _))
        .WillByDefault(DoAll(SetArgReferee<2>(resultjson), Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadFile(_, _, _)).Times(1);
    auto stt = stt::TextFromVoiceFactory<stt::googleapi::TextFromVoice>::create(
        shellMock, helpersMock, stt::language::polish);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}

TEST_F(TestStt, googleApiEmptyTextShellCmdAndHelpersCalled_ResultValid)
{
    std::string initialtext = "";
    double initialconfid = 0.;
    auto initialquality = (uint32_t)std::lround(100 * initialconfid);
    std::string resultjson = {
        "{\"result\":[{\"alternative\":[{\"transcript\":\"" + initialtext +
        "\","
        "\"confidence\":" +
        std::to_string(initialconfid) +
        "}],\"final\":true}],\"result_index\":0}"};
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    ON_CALL(*helpersMock, uploadFile(_, _, _))
        .WillByDefault(DoAll(SetArgReferee<2>(resultjson), Return(true)));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadFile(HasSubstr("lang=en-US"), _, _))
        .Times(1);
    auto stt = stt::TextFromVoiceFactory<stt::googleapi::TextFromVoice>::create(
        shellMock, helpersMock, stt::language::english);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}

TEST_F(TestStt,
       googleApiValidTextShellCmdAndHelpersCalled_languagesChangedAndRestored)
{
    std::string initialtext = "this is sample text!";
    double initialconfid = 0.825;
    auto initialquality = (uint32_t)std::lround(100 * initialconfid);
    std::string resultjson = {
        "{\"result\":[{\"alternative\":[{\"transcript\":\"" + initialtext +
        "\","
        "\"confidence\":" +
        std::to_string(initialconfid) +
        "}],\"final\":true}],\"result_index\":0}"};
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    ON_CALL(*helpersMock, uploadFile(_, _, _))
        .WillByDefault(DoAll(SetArgReferee<2>(resultjson), Return(true)));

    InSequence seq;
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadFile(HasSubstr("lang=en-US"), _, _))
        .Times(1);
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadFile(HasSubstr("lang=de-DE"), _, _))
        .Times(1);
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadFile(HasSubstr("lang=en-US"), _, _))
        .Times(1);

    auto stt = stt::TextFromVoiceFactory<stt::googleapi::TextFromVoice>::create(
        shellMock, helpersMock, stt::language::english);
    auto result1 = stt->listen();
    auto result2 = stt->listen(stt::language::german);
    auto result3 = stt->listen();

    EXPECT_EQ(std::make_pair(initialtext, initialquality), result1);
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result2, result3);
}

TEST_F(TestStt, googleApiShellCmdCalledAndNoMockedHelpersUsed_ListenThrows)
{
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto stt = stt::TextFromVoiceFactory<stt::googleapi::TextFromVoice>::create(
        shellMock, stt::language::english);
    EXPECT_THROW(stt->listen(), std::exception);
}

TEST_F(TestStt, googleCloudV1ShellCmdCalledAndNoMockedHelpersUsed_ListenThrows)
{
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto stt =
        stt::TextFromVoiceFactory<stt::googlecloud::v1::TextFromVoice>::create(
            shellMock, stt::language::english);
    EXPECT_THROW(stt->listen(), std::runtime_error);
}

TEST_F(
    TestStt,
    googleCloudV1ShellCmdCalledAndNoMockedHelpersUsed_ListenChangedLangThrows)
{
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto stt =
        stt::TextFromVoiceFactory<stt::googlecloud::v1::TextFromVoice>::create(
            shellMock, stt::language::english);
    EXPECT_THROW(stt->listen(stt::language::polish), std::runtime_error);
}

TEST_F(TestStt, googleCloudV2ShellCmdCalledAndNoMockedHelpersUsed_ListenThrows)
{
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto stt =
        stt::TextFromVoiceFactory<stt::googlecloud::v2::TextFromVoice>::create(
            shellMock, stt::language::english);
    EXPECT_THROW(stt->listen(), std::runtime_error);
}

TEST_F(
    TestStt,
    googleCloudV2ShellCmdCalledAndNoMockedHelpersUsed_ListenChangedLangThrows)
{
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto stt =
        stt::TextFromVoiceFactory<stt::googlecloud::v2::TextFromVoice>::create(
            shellMock, stt::language::english);
    EXPECT_THROW(stt->listen(stt::language::polish), std::runtime_error);
}
