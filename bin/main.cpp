#include <filesystem>
#include <../lib/tapes/FileTape.h>

#include "../lib/configs/handlers/Factories/AbstractBuilderHandlersFactory.h"
#include "../lib/tapes/factories/AbstractTapeFactory.h"
#include "../lib/configs/Config.h"
#include "../lib/tapes/factories/FileTapeFactory.h"
#include "TapeSorter.h"
#include "configs/EnvParser.h"
#include "configs/handlers/Factories/ConfigBuilderHandlersFactory.h"


bool inputIsBinary(int argc, char *argv[]) {
    if (argc == 2)
        return false;
    std::string flag = argv[1];
    return flag == "-b";
}

int main(int argc, char *argv[]) {
    std::filesystem::path envPath = std::filesystem::path(PROJECT_ROOT) / ".env";
    EnvParser envParser;
    Config::Builder builder;
    std::unique_ptr<AbstractBuilderHandlersFactory> handlerFactory
            = std::make_unique<ConfigBuilderHandlersFactory>();
    std::unique_ptr<Handler> handler = handlerFactory->createHandlersChain();
    builder = handler->handle(envParser.parse(envPath), builder);
    handlerFactory.reset();
    handler.reset();

    Config config = builder.build();

    std::shared_ptr<AbstractTape<int32_t> > input;

    if (argc < 2) {
        throw std::invalid_argument("missing input file path or output file path");
    }
    if (inputIsBinary(argc, argv)) {
        input = std::make_shared<FileTape<int32_t> >(
            std::string(argv[2]),
            config.readDataDelay,
            config.writeDataDelay,
            config.moveTapeDelay);
    } else {
        std::string filePath = argv[1];
        filePath += "_bin";
        input = std::make_shared<FileTape<int32_t> >(
            filePath,
            config.readDataDelay,
            config.writeDataDelay,
            config.moveTapeDelay);
        std::fstream file(filePath, std::ios::in);
        if (!file.is_open()) throw std::runtime_error("failed to open input file");
        input->convertFromStream(file);
    }

    std::shared_ptr<AbstractTape<int32_t> > output = std::make_shared<FileTape<int32_t> >(
        std::string(argv[argc - 1]),
        config.readDataDelay,
        config.writeDataDelay,
        config.moveTapeDelay);

    std::shared_ptr<AbstractTapeFactory<int32_t> > tapeFactory
            = std::make_shared<FileTapeFactory<int32_t> >(config);

    TapeSorter<int32_t> sorter(input, output, tapeFactory, config.maxMemoryCapacity);

    sorter.sort();

    output.reset();
    input.reset();
    tapeFactory.reset();



    // reader for result tape
    std::shared_ptr<AbstractTape<int32_t> > consoleTapeReader = std::make_shared<FileTape<int32_t> >(
        std::string(argv[argc - 1]),
        std::chrono::milliseconds(0),
        std::chrono::milliseconds(0),
        std::chrono::milliseconds(0));

    while (consoleTapeReader->hasNext()) {
        std::cout << consoleTapeReader->read() << " :From output" << std::endl;
        consoleTapeReader->next();
    }

    return 0;
}
