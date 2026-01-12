#include "crow.h"

int main() {
    crow::SimpleApp app;

    // Health routes
    CROW_ROUTE(app, "/health")
    ([]() {
        crow::json::wvalue x;
        x["status"] = "ok";
        x["version"] = "0.1.0";
        return x;
    });

    CROW_ROUTE(app, "/health/ready")
    ([]() {
        crow::json::wvalue x;
        x["status"] = "ready";
        x["version"] = "0.1.0";
        return x;
    });

    // User routes
    CROW_ROUTE(app, "/users")
    ([]() {
        crow::json::wvalue users;
        users[0]["id"] = 1;
        users[0]["name"] = "Alice";
        users[0]["email"] = "alice@example.com";
        users[1]["id"] = 2;
        users[1]["name"] = "Bob";
        users[1]["email"] = "bob@example.com";
        return users;
    });

    CROW_ROUTE(app, "/users/<int>")
    ([](int id) {
        crow::json::wvalue x;
        x["id"] = id;
        x["name"] = "Sample User";
        x["email"] = "user@example.com";
        return x;
    });

    CROW_ROUTE(app, "/users").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        crow::json::wvalue x;
        x["id"] = 1;
        x["name"] = body["name"].s();
        x["email"] = body["email"].s();
        return crow::response(201, x);
    });

    CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)
    ([](const crow::request& req, int id) {
        auto body = crow::json::load(req.body);
        crow::json::wvalue x;
        x["id"] = id;
        x["name"] = body["name"].s();
        x["email"] = body["email"].s();
        return x;
    });

    CROW_ROUTE(app, "/users/<int>").methods("DELETE"_method)
    ([](int id) {
        return crow::response(204);
    });

    CROW_ROUTE(app, "/users/<int>/posts")
    ([](int userId) {
        crow::json::wvalue posts;
        posts[0]["id"] = 1;
        posts[0]["userId"] = userId;
        posts[0]["title"] = "User Post";
        posts[0]["body"] = "Content";
        return posts;
    });

    // Post routes
    CROW_ROUTE(app, "/posts")
    ([]() {
        crow::json::wvalue posts;
        posts[0]["id"] = 1;
        posts[0]["userId"] = 1;
        posts[0]["title"] = "First Post";
        posts[0]["body"] = "Hello world";
        posts[1]["id"] = 2;
        posts[1]["userId"] = 1;
        posts[1]["title"] = "Second Post";
        posts[1]["body"] = "Another post";
        return posts;
    });

    CROW_ROUTE(app, "/posts/<int>")
    ([](int id) {
        crow::json::wvalue x;
        x["id"] = id;
        x["userId"] = 1;
        x["title"] = "Sample Post";
        x["body"] = "Post body";
        return x;
    });

    CROW_ROUTE(app, "/posts").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        crow::json::wvalue x;
        x["id"] = 1;
        x["userId"] = body["userId"].i();
        x["title"] = body["title"].s();
        x["body"] = body["body"].s();
        return crow::response(201, x);
    });

    app.port(8080).multithreaded().run();
}
