#include"Logg.h"
// int main()
// {
//     try
//     {
//         Logger logger("log.txt");
//
//         logger.log(LogLevel::DEBUG,"Starting application.");
//
//         int user_id = 42;
//         std::string action = "login";
//         double duration = 3.5;
//         std::string world = "World";
//
//         logger.log(LogLevel::DEBUG,"User {} performed {} in {} seconds.", user_id, action, duration);
//         logger.log(LogLevel::DEBUG,"Hello {}", world);
//         logger.log(LogLevel::DEBUG,"This is a message without placeholders.");
//         logger.log(LogLevel::DEBUG,"Multiple placeholders: {}, {}, {}.", 1, 2, 3);
//     }
//     catch (const std::exception& e)
//     {
//         std::cout<<e.what()<<std::endl;
//     }
//     return 0;
// }