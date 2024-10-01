#include "mocks/mock_helpers.hpp"
#include "mocks/mock_shell.hpp"
#include "stt/interfaces/googleapi.hpp"

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

TEST_F(TestStt, testSttValidTextShellCmdAndHelpersCalledResultValid)
{
    std::string initialtext = "sample text from voice";
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
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    EXPECT_CALL(*helpersMock, uploadFile(_, _, _)).Times(1);
    auto stt = stt::TextFromVoiceFactory<stt::googleapi::TextFromVoice>::create(
        shellMock, helpersMock, stt::language::english);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}

TEST_F(TestStt, testSttEmptyTextShellCmdAndHelpersCalledResultValid)
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
    EXPECT_CALL(*helpersMock, uploadFile(_, _, _)).Times(1);
    auto stt = stt::TextFromVoiceFactory<stt::googleapi::TextFromVoice>::create(
        shellMock, helpersMock, stt::language::english);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}

TEST_F(TestStt, testSttFactoryShellCmdCalledAndNoMockedHelpersThrow)
{
    ON_CALL(*shellMock, run(_)).WillByDefault(Return(0));
    EXPECT_CALL(*shellMock, run(_)).Times(1);
    auto stt = stt::TextFromVoiceFactory<stt::googleapi::TextFromVoice>::create(
        shellMock, stt::language::english);
    EXPECT_THROW(stt->listen(), std::exception);
}
