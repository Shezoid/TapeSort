#include "AbstractBuilderHandlersFactory.h"

class ConfigBuilderHandlersFactory : public AbstractBuilderHandlersFactory {
public:
    std::unique_ptr<Handler> createHandlersChain() override {
        std::function<void(Config::Builder &, std::size_t&)> maxMemorySetter =
                [](Config::Builder &builder, std::size_t& value) {
            builder.withMaxMemoryCapacity(value);
        };

        std::function<void(Config::Builder &, std::string&)> tempDirectorySetter =
                [](Config::Builder &builder, std::string& value) {
            builder.withTempDirectoryPath(value);
        };

        std::function<void(Config::Builder &, std::chrono::milliseconds&)> readDelaySetter =
                [](Config::Builder &builder, std::chrono::milliseconds value) {
                    builder.withReadDataDelay(value);
        };

        std::function<void(Config::Builder &, std::chrono::milliseconds&)> writeDelaySetter =
                [](Config::Builder &builder, std::chrono::milliseconds value) {
                    builder.withWriteDataDelay(value);
        };

        std::function<void(Config::Builder &, std::chrono::milliseconds&)> moveDelaySetter =
                [](Config::Builder &builder, std::chrono::milliseconds value) {
                    builder.withMoveDataDelay(value);
        };


        std::unique_ptr<Handler> maxMemoryCapacityHandler = std::make_unique<ConfigHandler<std::size_t>>("MAX_MEMORY_CAPACITY", maxMemorySetter);
        Handler *tempDirectoryHandler = new ConfigHandler("TEMP_DIRECTORY_PATH", tempDirectorySetter);
        Handler *readDataHandler = new ConfigHandler("READ_DATA_DELAY", readDelaySetter);
        Handler *writeDataHandler = new ConfigHandler("WRITE_DATA_DELAY", writeDelaySetter);
        Handler *moveDataHandler = new ConfigHandler("MOVE_TAPE_DELAY", moveDelaySetter);

        maxMemoryCapacityHandler->
        setNext(tempDirectoryHandler)->
        setNext(readDataHandler)->
        setNext(writeDataHandler)->
        setNext(moveDataHandler);

        return maxMemoryCapacityHandler;
    }
};
