add_executable(main
    main.cpp
    Message.cpp
    Server.cpp
    Client.cpp
)

add_library(parallel_stream
        Message.cpp
        Server.cpp
        Client.cpp
        )
target_link_libraries(main Boost::serialization Boost::log)
target_link_libraries(parallel_stream Boost::serialization Boost::log)
