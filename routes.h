// routes.h - Route setup for Crow application
#ifndef ROUTES_H
#define ROUTES_H

#include "crow.h"

inline void setup_routes(crow::SimpleApp& app) {
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
        if (id <= 0) {
            crow::json::wvalue error;
            error["error"] = "Invalid user ID";
            return crow::response(400, error);
        }
        if (id > 100) {
            crow::json::wvalue error;
            error["error"] = "User not found";
            return crow::response(404, error);
        }
        crow::json::wvalue x;
        x["id"] = id;
        x["name"] = "Sample User";
        x["email"] = "user@example.com";
        return crow::response(200, x);
    });

    CROW_ROUTE(app, "/users").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            crow::json::wvalue error;
            error["error"] = "Invalid JSON";
            return crow::response(400, error);
        }
        if (!body.has("name") || !body.has("email")) {
            crow::json::wvalue error;
            error["error"] = "Missing required fields: name, email";
            return crow::response(400, error);
        }
        crow::json::wvalue x;
        x["id"] = 1;
        x["name"] = body["name"].s();
        x["email"] = body["email"].s();
        return crow::response(201, x);
    });

    CROW_ROUTE(app, "/users/<int>").methods("PUT"_method)
    ([](const crow::request& req, int id) {
        if (id <= 0) {
            crow::json::wvalue error;
            error["error"] = "Invalid user ID";
            return crow::response(400, error);
        }
        if (id > 100) {
            crow::json::wvalue error;
            error["error"] = "User not found";
            return crow::response(404, error);
        }
        auto body = crow::json::load(req.body);
        if (!body) {
            crow::json::wvalue error;
            error["error"] = "Invalid JSON";
            return crow::response(400, error);
        }
        if (!body.has("name") || !body.has("email")) {
            crow::json::wvalue error;
            error["error"] = "Missing required fields: name, email";
            return crow::response(400, error);
        }
        crow::json::wvalue x;
        x["id"] = id;
        x["name"] = body["name"].s();
        x["email"] = body["email"].s();
        return crow::response(200, x);
    });

    CROW_ROUTE(app, "/users/<int>").methods("DELETE"_method)
    ([](int id) {
        if (id <= 0) {
            crow::json::wvalue error;
            error["error"] = "Invalid user ID";
            return crow::response(400, error);
        }
        if (id > 100) {
            crow::json::wvalue error;
            error["error"] = "User not found";
            return crow::response(404, error);
        }
        return crow::response(204);
    });

    CROW_ROUTE(app, "/users/<int>/posts")
    ([](int userId) {
        if (userId <= 0) {
            crow::json::wvalue error;
            error["error"] = "Invalid user ID";
            return crow::response(400, error);
        }
        if (userId > 100) {
            crow::json::wvalue error;
            error["error"] = "User not found";
            return crow::response(404, error);
        }
        crow::json::wvalue posts;
        posts[0]["id"] = 1;
        posts[0]["userId"] = userId;
        posts[0]["title"] = "User Post";
        posts[0]["body"] = "Content";
        return crow::response(200, posts);
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
        if (id <= 0) {
            crow::json::wvalue error;
            error["error"] = "Invalid post ID";
            return crow::response(400, error);
        }
        if (id > 100) {
            crow::json::wvalue error;
            error["error"] = "Post not found";
            return crow::response(404, error);
        }
        crow::json::wvalue x;
        x["id"] = id;
        x["userId"] = 1;
        x["title"] = "Sample Post";
        x["body"] = "Post body";
        return crow::response(200, x);
    });

    CROW_ROUTE(app, "/posts").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            crow::json::wvalue error;
            error["error"] = "Invalid JSON";
            return crow::response(400, error);
        }
        if (!body.has("userId") || !body.has("title") || !body.has("body")) {
            crow::json::wvalue error;
            error["error"] = "Missing required fields: userId, title, body";
            return crow::response(400, error);
        }
        crow::json::wvalue x;
        x["id"] = 1;
        x["userId"] = body["userId"].i();
        x["title"] = body["title"].s();
        x["body"] = body["body"].s();
        return crow::response(201, x);
    });
}

#endif // ROUTES_H
