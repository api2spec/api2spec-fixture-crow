# api2spec-fixture-crow

A sample Crow (C++) API fixture for testing api2spec.

## Requirements

- Docker and Docker Compose
- Or locally: CMake 3.15+, GCC/Clang with C++17, Boost, Asio

## Docker Development

### Run the production build

```bash
docker compose up app
```

The server will be available at http://localhost:8080

### Interactive development container

```bash
docker compose run --rm dev
```

Inside the container, build and run:

```bash
apt-get update && apt-get install -y cmake libboost-all-dev libasio-dev
cmake -B build
cmake --build build
./build/server
```

## Local Development

### Build

```bash
cmake -B build
cmake --build build
```

### Run

```bash
./build/server
```

## API Endpoints

### Health

- `GET /health` - Health check
- `GET /health/ready` - Readiness check

### Users

- `GET /users` - List all users
- `GET /users/:id` - Get user by ID
- `POST /users` - Create a user
- `PUT /users/:id` - Update a user
- `DELETE /users/:id` - Delete a user
- `GET /users/:id/posts` - Get posts by user

### Posts

- `GET /posts` - List all posts
- `GET /posts/:id` - Get post by ID
- `POST /posts` - Create a post

## Example Requests

```bash
# Health check
curl http://localhost:8080/health

# List users
curl http://localhost:8080/users

# Get user by ID
curl http://localhost:8080/users/1

# Create user
curl -X POST http://localhost:8080/users \
  -H "Content-Type: application/json" \
  -d '{"name": "Alice", "email": "alice@example.com"}'

# Update user
curl -X PUT http://localhost:8080/users/1 \
  -H "Content-Type: application/json" \
  -d '{"name": "Alice Updated", "email": "alice@example.com"}'

# Delete user
curl -X DELETE http://localhost:8080/users/1

# Get user posts
curl http://localhost:8080/users/1/posts

# List posts
curl http://localhost:8080/posts

# Create post
curl -X POST http://localhost:8080/posts \
  -H "Content-Type: application/json" \
  -d '{"userId": 1, "title": "My Post", "body": "Hello world"}'
```
