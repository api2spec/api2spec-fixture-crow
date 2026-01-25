// Integration tests for Crow API
#include <gtest/gtest.h>
#include "crow.h"
#include "../routes.h"

class HealthApiTest : public ::testing::Test {
protected:
    crow::SimpleApp app;

    void SetUp() override {
        setup_routes(app);
        app.validate();
    }
};

class UserApiTest : public ::testing::Test {
protected:
    crow::SimpleApp app;

    void SetUp() override {
        setup_routes(app);
        app.validate();
    }
};

class PostApiTest : public ::testing::Test {
protected:
    crow::SimpleApp app;

    void SetUp() override {
        setup_routes(app);
        app.validate();
    }
};

// =============================================================================
// Health Endpoint Tests
// =============================================================================

TEST_F(HealthApiTest, GetHealth_ReturnsOk) {
    crow::request req;
    req.url = "/health";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["status"].s(), "ok");
    EXPECT_EQ(body["version"].s(), "0.1.0");
}

TEST_F(HealthApiTest, GetHealthReady_ReturnsReady) {
    crow::request req;
    req.url = "/health/ready";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["status"].s(), "ready");
    EXPECT_EQ(body["version"].s(), "0.1.0");
}

// =============================================================================
// User Endpoint Tests
// =============================================================================

TEST_F(UserApiTest, GetUsers_ReturnsList) {
    crow::request req;
    req.url = "/users";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body.size(), 2u);
    EXPECT_EQ(body[0]["id"].i(), 1);
    EXPECT_EQ(body[0]["name"].s(), "Alice");
    EXPECT_EQ(body[1]["id"].i(), 2);
    EXPECT_EQ(body[1]["name"].s(), "Bob");
}

TEST_F(UserApiTest, GetUserById_ReturnsUser) {
    crow::request req;
    req.url = "/users/1";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["id"].i(), 1);
    EXPECT_EQ(body["name"].s(), "Sample User");
    EXPECT_EQ(body["email"].s(), "user@example.com");
}

TEST_F(UserApiTest, GetUserById_WhenInvalidId_Returns400) {
    crow::request req;
    req.url = "/users/0";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Invalid user ID");
}

TEST_F(UserApiTest, GetUserById_WhenNotFound_Returns404) {
    crow::request req;
    req.url = "/users/999";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 404);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "User not found");
}

TEST_F(UserApiTest, CreateUser_WithValidData_Returns201) {
    crow::request req;
    req.url = "/users";
    req.method = crow::HTTPMethod::POST;
    req.body = R"({"name": "Charlie", "email": "charlie@example.com"})";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 201);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["id"].i(), 1);
    EXPECT_EQ(body["name"].s(), "Charlie");
    EXPECT_EQ(body["email"].s(), "charlie@example.com");
}

TEST_F(UserApiTest, CreateUser_WithInvalidJson_Returns400) {
    crow::request req;
    req.url = "/users";
    req.method = crow::HTTPMethod::POST;
    req.body = "invalid json";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Invalid JSON");
}

TEST_F(UserApiTest, CreateUser_WithMissingFields_Returns400) {
    crow::request req;
    req.url = "/users";
    req.method = crow::HTTPMethod::POST;
    req.body = R"({"name": "Charlie"})";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Missing required fields: name, email");
}

TEST_F(UserApiTest, UpdateUser_WithValidData_Returns200) {
    crow::request req;
    req.url = "/users/1";
    req.method = crow::HTTPMethod::PUT;
    req.body = R"({"name": "Alice Updated", "email": "alice.updated@example.com"})";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["id"].i(), 1);
    EXPECT_EQ(body["name"].s(), "Alice Updated");
    EXPECT_EQ(body["email"].s(), "alice.updated@example.com");
}

TEST_F(UserApiTest, UpdateUser_WhenNotFound_Returns404) {
    crow::request req;
    req.url = "/users/999";
    req.method = crow::HTTPMethod::PUT;
    req.body = R"({"name": "Test", "email": "test@example.com"})";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 404);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "User not found");
}

TEST_F(UserApiTest, UpdateUser_WithInvalidJson_Returns400) {
    crow::request req;
    req.url = "/users/1";
    req.method = crow::HTTPMethod::PUT;
    req.body = "invalid json";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Invalid JSON");
}

TEST_F(UserApiTest, UpdateUser_WithMissingFields_Returns400) {
    crow::request req;
    req.url = "/users/1";
    req.method = crow::HTTPMethod::PUT;
    req.body = R"({"name": "Alice"})";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Missing required fields: name, email");
}

TEST_F(UserApiTest, DeleteUser_ReturnsNoContent) {
    crow::request req;
    req.url = "/users/1";
    req.method = crow::HTTPMethod::DELETE;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 204);
}

TEST_F(UserApiTest, DeleteUser_WhenNotFound_Returns404) {
    crow::request req;
    req.url = "/users/999";
    req.method = crow::HTTPMethod::DELETE;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 404);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "User not found");
}

TEST_F(UserApiTest, DeleteUser_WhenInvalidId_Returns400) {
    crow::request req;
    req.url = "/users/0";
    req.method = crow::HTTPMethod::DELETE;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Invalid user ID");
}

TEST_F(UserApiTest, GetUserPosts_ReturnsPosts) {
    crow::request req;
    req.url = "/users/1/posts";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body.size(), 1u);
    EXPECT_EQ(body[0]["id"].i(), 1);
    EXPECT_EQ(body[0]["userId"].i(), 1);
    EXPECT_EQ(body[0]["title"].s(), "User Post");
}

TEST_F(UserApiTest, GetUserPosts_WhenUserNotFound_Returns404) {
    crow::request req;
    req.url = "/users/999/posts";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 404);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "User not found");
}

// =============================================================================
// Post Endpoint Tests
// =============================================================================

TEST_F(PostApiTest, GetPosts_ReturnsList) {
    crow::request req;
    req.url = "/posts";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body.size(), 2u);
    EXPECT_EQ(body[0]["id"].i(), 1);
    EXPECT_EQ(body[0]["title"].s(), "First Post");
    EXPECT_EQ(body[1]["id"].i(), 2);
    EXPECT_EQ(body[1]["title"].s(), "Second Post");
}

TEST_F(PostApiTest, GetPostById_ReturnsPost) {
    crow::request req;
    req.url = "/posts/1";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 200);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["id"].i(), 1);
    EXPECT_EQ(body["userId"].i(), 1);
    EXPECT_EQ(body["title"].s(), "Sample Post");
    EXPECT_EQ(body["body"].s(), "Post body");
}

TEST_F(PostApiTest, GetPostById_WhenInvalidId_Returns400) {
    crow::request req;
    req.url = "/posts/0";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Invalid post ID");
}

TEST_F(PostApiTest, GetPostById_WhenNotFound_Returns404) {
    crow::request req;
    req.url = "/posts/999";
    req.method = crow::HTTPMethod::GET;

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 404);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Post not found");
}

TEST_F(PostApiTest, CreatePost_WithValidData_Returns201) {
    crow::request req;
    req.url = "/posts";
    req.method = crow::HTTPMethod::POST;
    req.body = R"({"userId": 1, "title": "New Post", "body": "Post content"})";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 201);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["id"].i(), 1);
    EXPECT_EQ(body["userId"].i(), 1);
    EXPECT_EQ(body["title"].s(), "New Post");
    EXPECT_EQ(body["body"].s(), "Post content");
}

TEST_F(PostApiTest, CreatePost_WithInvalidJson_Returns400) {
    crow::request req;
    req.url = "/posts";
    req.method = crow::HTTPMethod::POST;
    req.body = "invalid json";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Invalid JSON");
}

TEST_F(PostApiTest, CreatePost_WithMissingFields_Returns400) {
    crow::request req;
    req.url = "/posts";
    req.method = crow::HTTPMethod::POST;
    req.body = R"({"userId": 1, "title": "New Post"})";
    req.add_header("Content-Type", "application/json");

    crow::response res;
    app.handle(req, res);

    EXPECT_EQ(res.code, 400);
    auto body = crow::json::load(res.body);
    EXPECT_TRUE(body);
    EXPECT_EQ(body["error"].s(), "Missing required fields: userId, title, body");
}

// =============================================================================
// Main
// =============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
