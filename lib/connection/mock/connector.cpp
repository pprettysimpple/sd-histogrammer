#include "connector.h"

TFakeConnector::TFakeConnector() noexcept
: Counter(1)
{
}

uint64_t TFakeConnector::GetHistImpl(const char* hashtag, uint64_t fromTs, uint64_t toTs) {
    return std::string(hashtag).size() * Counter++;
}
