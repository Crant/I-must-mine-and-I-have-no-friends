#include "Tiles.h"
#include "../Include/RandomGen.h"
#include "../Maths.h"

using namespace IMM;

void Tiles::LoadTiles()
{
	mTypeToData[TileType::Dirt] = TileData(1.0f, 1.0f);
	mTypeToData[TileType::Granite] = TileData(100.0f, 100.0f);
}
TileData Tiles::GetData(TileType* tile)
{
	return mTypeToData[*tile];
}
void AllBlocks::LoadBlocks()
{
	//BlockStats* magnesite = new BlockStats();
	//magnesite->sType = "Mineral";
	//magnesite->cCommonOre = TileProperties::Magnesium_Carbonate;
	//magnesite->cBadUncommonOre = TileProperties::Nickel;
	//magnesite->cGoodUncommonOre = TileProperties::Magnesium;
	//magnesite->cBadRareOre = TileProperties::Iron;
	//magnesite->cGoodRareOre = TileProperties::Cobalt;

	//mBlockDesc[TileType::Magnesite] = magnesite;
}

std::unique_ptr<BlockYield> AllBlocks::GetBlockYield(float fPerlinSeed)
{
	auto yieldPtr = std::make_unique<BlockYield>();

	float fCommonYield = Maths::BellCurve2(fPerlinSeed, 0.3f, 0.7f);
	yieldPtr->sCommonYield = *CalculateBlockYield(fCommonYield);

	float fGoodUncommonYield = Maths::BellCurve2(fPerlinSeed, 0.5f, 0.7f);
	yieldPtr->sGoodUncommonYield = *CalculateBlockYield(fGoodUncommonYield);

	float fBadUncommonYield = Maths::BellCurve2(fPerlinSeed, 0.3f, 0.5f);
	yieldPtr->sBadUncommonYield = *CalculateBlockYield(fBadUncommonYield);

	float fGoodRareYield = Maths::BellCurve2(fPerlinSeed, 0.6f, 0.8f);
	yieldPtr->sGoodRareYield = *CalculateBlockYield(fGoodRareYield);

	float fBadRareYield = Maths::BellCurve2(fPerlinSeed, 0.2f, 0.4f);
	yieldPtr->sBadRareYield = *CalculateBlockYield(fBadRareYield);

	return yieldPtr;
}
std::unique_ptr<std::string> AllBlocks::CalculateBlockYield(float fRange)
{
	std::unique_ptr<std::string> sValue = std::make_unique<std::string>();
	*sValue = "N/A";
	if (fRange > 0.9f)
	{
		*sValue = "Very High";
	}
	else if (fRange > 0.7f)
	{
		*sValue = "High";
	}
	else if (fRange > 0.5f)
	{
		*sValue = "Medium";
	}
	else if (fRange > 0.3f)
	{
		*sValue = "Low";
	}
	else
	{
		*sValue = "Very Low";
	}
	return sValue;
}