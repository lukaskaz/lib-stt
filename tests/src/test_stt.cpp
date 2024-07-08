#include "mocks/mock_command.hpp"
#include "speechtotext.hpp"

using namespace testing;

class TestStt : public Test
{
  public:
    std::shared_ptr<NiceMock<CommandsMock>> commandMock =
        std::make_shared<NiceMock<CommandsMock>>();

  protected:
    void SetUp() override
    {}

    void TearDown() override
    {}
};

TEST_F(TestStt, testSttValidTextShellCmdCalledTwice)
{
    std::string initialtext = "sample text from voice";
    double initialconfid = 0.825;
    auto initialquality = (uint32_t)std::lround(100 * initialconfid);
    std::vector<std::string> sttjson = {
        "{\"result\":[{\"alternative\":[{\"transcript\":\"" + initialtext +
        "\","
        "\"confidence\":" +
        std::to_string(initialconfid) +
        "}],\"final\":true}],\"result_index\":0}"};
    ON_CALL(*commandMock, run(_)).WillByDefault(Return(0));
    ON_CALL(*commandMock, run(_, _))
        .WillByDefault(DoAll(SetArgReferee<1>(sttjson), Return(0)));
    EXPECT_CALL(*commandMock, run(_)).Times(1);
    EXPECT_CALL(*commandMock, run(_, _)).Times(1);
    auto stt = std::make_unique<stt::TextFromVoice>(commandMock,
                                                    stt::language::english);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}

TEST_F(TestStt, testSttEmptyTextShellCmdCalledTwice)
{
    std::string initialtext = "";
    double initialconfid = 0.;
    auto initialquality = (uint32_t)std::lround(100 * initialconfid);
    std::vector<std::string> sttjson = {
        "{\"result\":[{\"alternative\":[{\"transcript\":\"" + initialtext +
        "\","
        "\"confidence\":" +
        std::to_string(initialconfid) +
        "}],\"final\":true}],\"result_index\":0}"};
    ON_CALL(*commandMock, run(_)).WillByDefault(Return(0));
    ON_CALL(*commandMock, run(_, _))
        .WillByDefault(DoAll(SetArgReferee<1>(sttjson), Return(0)));
    EXPECT_CALL(*commandMock, run(_)).Times(1);
    EXPECT_CALL(*commandMock, run(_, _)).Times(1);
    auto stt = std::make_unique<stt::TextFromVoice>(commandMock,
                                                    stt::language::english);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}

TEST_F(TestStt, testSttFactoryValidTextShellCmdCalledTwice)
{
    std::string initialtext = "sample text from voice";
    double initialconfid = 0.825;
    auto initialquality = (uint32_t)std::lround(100 * initialconfid);
    std::vector<std::string> sttjson = {
        "{\"result\":[{\"alternative\":[{\"transcript\":\"" + initialtext +
        "\","
        "\"confidence\":" +
        std::to_string(initialconfid) +
        "}],\"final\":true}],\"result_index\":0}"};
    ON_CALL(*commandMock, run(_)).WillByDefault(Return(0));
    ON_CALL(*commandMock, run(_, _))
        .WillByDefault(DoAll(SetArgReferee<1>(sttjson), Return(0)));
    EXPECT_CALL(*commandMock, run(_)).Times(1);
    EXPECT_CALL(*commandMock, run(_, _)).Times(1);
    auto stt =
        stt::TextFromVoiceFactory::create(commandMock, stt::language::english);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}

TEST_F(TestStt, testSttFactoryEmptyTextShellCmdCalledTwice)
{
    std::string initialtext = "";
    double initialconfid = 0.;
    auto initialquality = (uint32_t)std::lround(100 * initialconfid);
    std::vector<std::string> sttjson = {
        "{\"result\":[{\"alternative\":[{\"transcript\":\"" + initialtext +
        "\","
        "\"confidence\":" +
        std::to_string(initialconfid) +
        "}],\"final\":true}],\"result_index\":0}"};
    ON_CALL(*commandMock, run(_)).WillByDefault(Return(0));
    ON_CALL(*commandMock, run(_, _))
        .WillByDefault(DoAll(SetArgReferee<1>(sttjson), Return(0)));
    EXPECT_CALL(*commandMock, run(_)).Times(1);
    EXPECT_CALL(*commandMock, run(_, _)).Times(1);
    auto stt =
        stt::TextFromVoiceFactory::create(commandMock, stt::language::english);
    auto [text, quality] = stt->listen();

    EXPECT_EQ(text, initialtext);
    EXPECT_EQ(quality, initialquality);
}
