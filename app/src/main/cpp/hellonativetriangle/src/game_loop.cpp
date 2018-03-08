#include "game_loop.h"

GameLoop::~GameLoop() {
    mutex_.lock();
    msg_ = Message::kStop;
    mutex_.unlock();
    condition_var_.notify_all();
    thread_.join();
}

void GameLoop::Play() {
    if (msg_ == Message::kPlay) {
        return;
    }
    if (!started_once_) {
        thread_ = thread{thread_callback_, this};
        started_once_ = true;
        return;
    }
    mutex_.lock();
    msg_ = Message::kPlay;
    mutex_.unlock();
    condition_var_.notify_all();
}

void GameLoop::Pause() {
    mutex_.lock();
    msg_ = Message::kPause;
    mutex_.unlock();
}

void GameLoop::ChangeRenderer(unique_ptr<Renderer> renderer) {
    mutex_.lock();
    msg_ = Message::kInitialize;
    renderer_ = move(renderer);
    mutex_.unlock();
}

void GameLoop::Loop() {
    while (true) {

        switch (msg_) {
            case Message::kInitialize :
                mutex_.lock();
                renderer_->Initialize();
                msg_ = Message::kPlay;
                mutex_.unlock();
                break;
            case Message::kPause : {
                unique_lock<mutex> lock{mutex_};
                condition_var_.wait(lock);
                break;
            }
            case Message::kStop :
                mutex_.lock();
                renderer_->Destroy();
                mutex_.unlock();
                return;
            default:
                break;
        }

        if (msg_ == Message::kPlay) {
            renderer_->DrawFrame();
        }
    }
}

void GameLoop::thread_callback_(GameLoop *game_loop) { game_loop->Loop(); }