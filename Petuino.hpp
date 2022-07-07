#pragma once

const float TIME_MULTIPLIER = 100;
const float SECONDS_FRAME = 0.04;

const float FOOD_RATE = -TIME_MULTIPLIER * SECONDS_FRAME / 100;
const float HAPPY_RATE = -TIME_MULTIPLIER * SECONDS_FRAME / 100;
const float SLEEP_RATE = -TIME_MULTIPLIER * SECONDS_FRAME / 100;

const float LIFE_WIN_RATE = TIME_MULTIPLIER * SECONDS_FRAME / 100;
const float LIFE_LOSS_RATE = -TIME_MULTIPLIER * SECONDS_FRAME / 200;
const float LIFE_LOSS_SICK = -TIME_MULTIPLIER * SECONDS_FRAME / 100;
const float HAPPY_LOSS_SICK = -TIME_MULTIPLIER * SECONDS_FRAME / 100;

const float SICKNESS_POO_RATE = TIME_MULTIPLIER * SECONDS_FRAME / 100;

const float FOOD_FEED = 25;
const float LIFE_FULL_FEED = -5;

const float HAPPY_PLAY = 25;
const float SLEEP_PLAY = -5;

const float FOOD_MEDICINE = -5;
const float HAPPY_MEDICINE = -15;

enum MENU_ACCION
{
	FOOD,
	PLAY,
	CLEAN,
	MEDICINE,
	DISCIPLINE,
	LIGHT
};

class PetPoint
{
public:
	float X;
	float Y;

	PetPoint() : X(0), Y(0) {}
	PetPoint(float x, float y) : X(x), Y(y) {}
};


class PetuinoPooManager
{
public:
	PetuinoPooManager()
	{
		pop_triggers[0] = 0;
		pop_triggers[1] = 0;
		pop_triggers[2] = 0;
	}

	void Trigger()
	{
		for(auto index = 0; index < 3; index++)
		{
			if(pop_triggers[index] == 0)
			{
				pop_triggers[index] = 25 * 3;
				return;
			}
		}
	}

	int Update()
	{
		int result = 0;
		for(auto index = 0; index < 3; index++)
		{
			if(pop_triggers[index] > 0)
			{
				pop_triggers[index] --;
				if(pop_triggers[index] == 0) result++;
			}
		}
		return result;
	}

private:
	unsigned long pop_triggers[3];
};

class PetuinoBehavior
{
};

class Petuino
{

public:

	float GetX()
	{
		if(position.X > 10) return 10;
		if(position.X < -10) return -10;
		return position.X;
	}

	float GetY()
	{
		return 0;
	}

	float GetFood()
	{
		return Food;
	}

	float GetHappy()
	{
		return Happy;
	}

	float GetSleep()
	{
		return Sleep;
	}

	float GetLife()
	{
		return Life;
	}

	bool GetIsSleep()
	{
		return isSleep;
	}

	float GetSickness()
	{
		return Sickness;
	}

	bool GetIsSick()
	{
		return Sickness == 100;
	}

	bool GetIsAlive()
	{
		return isAlive;
	}

	bool GetIsLightOn()
	{
		return isLightOn;
	}

	int GetPoo()
	{
		return poo_counter;
	}

	bool GetRequiresAttention()
	{
		return GetFood() < 25
			|| GetHappy() < 25
			|| GetSleep() < 25 && GetIsLightOn()
			|| GetLife() < 25
			|| GetIsSick()
			|| GetPoo() > 0;
	}

	void Feed()
	{
		if(isSleep) return;
		if(Food > 95) IncreaseLife(LIFE_FULL_FEED);
		IncreaseFood(FOOD_FEED);
		petuinoPooManager.Trigger();
	}

	void Play()
	{
		if(isSleep) return;
		if(Food < 25) return;
		IncreaseHappy(HAPPY_PLAY);
		IncreaseSleep(SLEEP_PLAY);
	}

	void Clean()
	{
		poo_counter--;
	}

	void Medicine()
	{
		IncreaseFood(FOOD_MEDICINE);
		IncreaseHappy(HAPPY_MEDICINE);
		Sickness = 0;
	}

	void Discipline()
	{
		if(isSleep) return;
	}

	void SwapLight()
	{
		isLightOn = !isLightOn;
	}

	void Update()
	{
		if(Life == 0)
		{
			isAlive = false;
			return;
		}

		IncreaseFood(FOOD_RATE);
		IncreaseHappy(HAPPY_RATE);

		if(Food < 25) IncreaseLife(LIFE_LOSS_RATE);
		if(Food > 75 && Happy > 75) IncreaseLife(LIFE_WIN_RATE);

		UpdateIsSleep();
		if(isSleep)
		{
			if(isLightOn) IncreaseSleep(-SLEEP_RATE / 3);
			else IncreaseSleep(-SLEEP_RATE);
		}
		else
		{
			IncreaseSleep(SLEEP_RATE);
		}

		UpdatePoo();

		UpdateSickness();
		if(GetIsSick())
		{
			IncreaseLife(LIFE_LOSS_SICK);
			IncreaseLife(HAPPY_LOSS_SICK);
		}

		UpdateIsSleep();
		UpdateMove();
	}

private:
	float Food = 100.0;
	float Happy = 100.0;
	float Sleep = 100.0;
	float Life = 100.0;
	float Sickness = 0.0;

	bool isLightOn = true;
	bool isSleep = false;
	bool isAlive = true;

	float deltaX = 0.3;
	PetPoint position;
	PetPoint speed = PetPoint(0.3, 0);

	int poo_counter = 0;
	PetuinoPooManager petuinoPooManager;

	void UpdateMove()
	{
		position.X += speed.X;
		if(position.X > 15) speed.X *= -1;
		if(position.X < -15) speed.X *= -1;
	}

	void UpdatePoo()
	{
		auto newPoo = petuinoPooManager.Update();
		poo_counter = (poo_counter + newPoo);
		if(poo_counter > 3) poo_counter = 3;
	}

	void UpdateSickness()
	{
		auto delta = (GetPoo() - 1) * SICKNESS_POO_RATE;
		IncreaseSickness(delta);
	}

	void UpdateIsSleep()
	{
		if(isLightOn)
		{
			if(Sleep < 5) isSleep = true;
			if(Sleep > 75) isSleep = false;
		}
		else
		{
			if(Sleep < 25) isSleep = true;
			if(Sleep > 99) isSleep = false;
		}
	}

	void IncreaseFood(float value) { IncreaseIndicator(Food, value); }

	void IncreaseHappy(float value) { IncreaseIndicator(Happy, value); }

	void IncreaseSleep(float value) { IncreaseIndicator(Sleep, value); }

	void IncreaseLife(float value) { IncreaseIndicator(Life, value); }

	void IncreaseSickness(float value) { IncreaseIndicator(Sickness, value); }

	void IncreaseIndicator(float& indicator, float delta)
	{
		indicator += delta;
		if(indicator < 0) indicator = 0;
		if(indicator > 100) indicator = 100;
	}
};
