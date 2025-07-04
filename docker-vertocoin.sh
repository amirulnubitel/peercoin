#!/bin/bash

# Vertocoin Docker Management Script
# Usage: ./docker-vertocoin.sh [command]

set -e

COMPOSE_FILE="docker-compose.yml"
IMAGE_NAME="vertocoin"
CONTAINER_NAME="vertocoin-node"

show_help() {
    echo "Vertocoin Docker Management Script"
    echo ""
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  build          Build the Vertocoin Docker image"
    echo "  start          Start Vertocoin node"
    echo "  stop           Stop Vertocoin node"
    echo "  restart        Restart Vertocoin node"
    echo "  logs           Show container logs"
    echo "  status         Show container status"
    echo "  cli [args]     Run vertocoin-cli command"
    echo "  shell          Open shell in running container"
    echo "  clean          Remove containers and images"
    echo "  help           Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 build                    # Build the image"
    echo "  $0 start                    # Start the node"
    echo "  $0 cli getblockchaininfo    # Get blockchain info"
    echo "  $0 logs -f                  # Follow logs"
}

build_image() {
    echo "Building Vertocoin Docker image..."
    docker compose build
    echo "Build completed!"
}

start_node() {
    echo "Starting Vertocoin node..."
    docker compose up -d
    echo "Vertocoin node started!"
    echo "Waiting for node to be ready..."
    sleep 10
    docker compose logs vertocoind
}

stop_node() {
    echo "Stopping Vertocoin node..."
    docker compose stop vertocoind
    echo "Vertocoin node stopped!"
}

restart_node() {
    echo "Restarting Vertocoin node..."
    docker compose restart vertocoind
    echo "Vertocoin node restarted!"
}

show_logs() {
    docker compose logs "${@:2}" vertocoind
}

show_status() {
    echo "Container Status:"
    docker compose ps
    echo ""
    echo "Node Info (if running):"
    if docker compose exec vertocoind vertocoin-cli -rpcuser=vertouser -rpcpassword=vertouser2025 getblockchaininfo 2>/dev/null; then
        echo "Node is running and responsive"
    else
        echo "Node is not responding or not running"
    fi
}

run_cli() {
    if [ $# -eq 1 ]; then
        docker compose exec vertocoind vertocoin-cli -rpcuser=vertouser -rpcpassword=vertouser2025 --help
    else
        docker compose exec vertocoind vertocoin-cli -rpcuser=vertouser -rpcpassword=vertouser2025 "${@:2}"
    fi
}

open_shell() {
    docker compose exec vertocoind bash
}

clean_all() {
    echo "Stopping and removing containers..."
    docker compose down
    echo "Removing images..."
    docker rmi "${IMAGE_NAME}" 2>/dev/null || echo "Image not found"
    echo "Cleaning up unused volumes (optional)..."
    read -p "Do you want to remove data volumes? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        docker compose down -v
        echo "Volumes removed!"
    fi
    echo "Cleanup completed!"
}

# Main script logic
case "${1:-help}" in
    build)
        build_image
        ;;
    start)
        start_node
        ;;
    stop)
        stop_node
        ;;
    restart)
        restart_node
        ;;
    logs)
        show_logs "$@"
        ;;
    status)
        show_status
        ;;
    cli)
        run_cli "$@"
        ;;
    shell)
        open_shell
        ;;
    clean)
        clean_all
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        echo "Unknown command: $1"
        echo ""
        show_help
        exit 1
        ;;
esac
