// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CharacterInfo.h"

FCharacterDefaultInfo UCharacterInfo::GetClassDefaultInfo(ECharacterInfo CharacterInfo)
{
	return CharacterInformation.FindChecked(CharacterInfo);
}