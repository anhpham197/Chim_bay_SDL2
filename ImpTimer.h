﻿#pragma once

#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	void stop();
	void paused();
	void resume();
	
	int get_ticks();       // hàm lấy thời gian đã trôi qua

	bool is_started();
	bool is_paused();

private:
	int start_tick_;    // thời điểm bắt đầu chạy
	int paused_tick_;         // thời điểm tạm dừng
	bool is_paused_;
	bool is_started_;
};

#endif // !IMP_TIMER_H_

