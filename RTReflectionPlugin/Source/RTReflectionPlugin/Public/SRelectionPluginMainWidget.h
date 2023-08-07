// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * This class encapsulates all the implementation of the real-time reflection plugin.
 */
class RTREFLECTIONPLUGIN_API SRelectionPluginMainWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRelectionPluginMainWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	enum InitializationOptions {
		LUMEN,
		SSRefection,
		Capture
	} mInitializationOption;

	bool bPostprocessingUnbound;

	TAttribute<FText> mInitializationOptionButtonTitle;

	bool bSphereCapture;

	int32 mReflectionQuality;
	TAttribute<TOptional<int32>> mReflectionQualityControl;

	int16 mReflectionMapSize;
	TAttribute<TOptional<int16>> mReflectionMapSizeControl;

	FString mGamePlatformName;


	void InitializationSession(const FArguments& InArgs);

	FReply OnInitializationClickedEvent();

	void OnUnboundCheckBoxStateChanged(ECheckBoxState NewState);
	ECheckBoxState IsUnboundCheckBoxChecked() const;

	FReply OnAddsCaptureClickedEvent();
	void OnCaptureCheckBoxStateChanged(ECheckBoxState NewState);
	ECheckBoxState IsCaptureCheckBoxChecked() const;

	FReply OnInitOptionLumenClickedEvent();
	FReply OnInitOptionSSReflectionClickedEvent();
	FReply OnInitOptionCaptureClickedEvent();
	FText GetInitializationButtonTitle() const;
	float GetLumenReflectionQuality();
	float GetSSReflectionQuality();

	

	FReply OnAddTestObjectClickedEvent();

	void SetReflectionQuality(int32 val);
	TOptional<int32> GetReflectionQuality() const;

	void SetReflectionMapSize(int16 val);
	TOptional<int16> GetReflectionMapSize() const;

	AActor* FindActor(TSubclassOf<AActor> ActorClass);

	AActor* AddActor(TSubclassOf<AActor> ActorClass);

	bool IsMobilePlatform() const;
};
