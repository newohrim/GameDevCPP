#pragma once

class BoardState;

class AlphaBetaPruning
{
public:
	AlphaBetaPruning();

	const BoardState* AlphaBetaDecide(const BoardState* Root, int MaxDepth);

	float AlphaBetaMax(const BoardState* Node, int Depth, float Alpha, float Beta);
	
	float AlphaBetaMin(const BoardState* Node, int Depth, float Alpha, float Beta);

private:
	
};

