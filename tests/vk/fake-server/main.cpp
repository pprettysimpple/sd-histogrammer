#include "server.h"

int main() {
    TFakeServer server(O_POLL, {});
    server.Listen();
}