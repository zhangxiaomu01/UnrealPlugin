// Fill out your copyright notice in the Description page of Project Settings.


#include "SRelectionPluginMainWidget.h"
#include "SlateOptMacros.h"
#include "EditorStyleSet.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SphereReflectionCapture.h"
#include "Engine/BoxReflectionCapture.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/RendererSettings.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRelectionPluginMainWidget::Construct(const FArguments& InArgs)
{
	mGamePlatformName = UGameplayStatics::GetPlatformName();
	UE_LOG(LogTemp, Warning, TEXT("Game platform name: %s"), *mGamePlatformName);
	bPostprocessingUnbound = true;
	bSphereCapture = true;
	if (IsMobilePlatform()) {
		mInitializationOption = InitializationOptions::SSRefection;
	}
	else {
		mInitializationOption = InitializationOptions::LUMEN;
	}
	mReflectionQuality = 50.0f;
	mReflectionMapSize = 128;

	mInitializationOptionButtonTitle.Bind(this, &SRelectionPluginMainWidget::GetInitializationButtonTitle);

	mReflectionQualityControl.Bind(this, &SRelectionPluginMainWidget::GetReflectionQuality);

	mReflectionMapSizeControl.Bind(this, &SRelectionPluginMainWidget::GetReflectionMapSize);

	InitializationSession(InArgs);
}

//SRelectionPluginMainWidget::SRelectionPluginMainWidget()
//{
//	//static ConstructorHelpers::FObjectFinder<AActor> MeshRef(TEXT("/Game/ReflectionPlugin/Blueprint/ReflectionDecoBp01.ReflectionDecoBp01"));
//	ConstructorHelpers::FClassFinder<AActor> NameOfResult(TEXT("/Game/ReflectionPlugin/Blueprint/ReflectionDecoBp01.ReflectionDecoBp01"));
//	//AActor* mesh = MeshRef.Object;
//
//	if (ensure(NameOfResult.Class)) {
//		UE_LOG(LogTemp, Warning, TEXT("Class founda!"));
//	}
//}


void SRelectionPluginMainWidget::InitializationSession(const FArguments& InArgs)
{
	float topLevelPadding = 10.f;

	ChildSlot
		[
			SNew(SVerticalBox)

		// Initialization title
		+ SVerticalBox::Slot().Padding(topLevelPadding)
		.AutoHeight()
		[
			SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Initialization Reflection System"))
				.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
			]
		]

		// Initialization button
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Text(FText::FromString("Initialize"))
			.OnClicked(FOnClicked::CreateSP(this, &SRelectionPluginMainWidget::OnInitializationClickedEvent))
			]

		// Initialization options + unbound
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Options"))
					]
				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top).Padding(15.0f, 0.0f)
					[
						SNew(SComboButton)
						.HAlign(HAlign_Left)
						.HasDownArrow(true)
						.ButtonContent()
						[
							SNew(STextBlock).Text		(mInitializationOptionButtonTitle)
						]
						.MenuContent()
							[
								SNew(SVerticalBox)
									+SVerticalBox::Slot()
										.AutoHeight()
										[
											SNew(SButton)
											.Text(FText::FromString(TEXT("Lumen")))
											.OnClicked(this, &SRelectionPluginMainWidget::OnInitOptionLumenClickedEvent)
										]

								+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SButton)
										.Text(FText::FromString(TEXT("SSReflection + Capture"))).OnClicked(this, &SRelectionPluginMainWidget::OnInitOptionSSReflectionClickedEvent)
									]

								+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SButton)
										.Text(FText::FromString(TEXT("Capture only"))).OnClicked(this, &SRelectionPluginMainWidget::OnInitOptionCaptureClickedEvent)
									]
							]

					]

				// Whether the post-processing should be unbound
				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Infinite Extent"))
					]

				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top)
					[
						SNew(SCheckBox)
						.OnCheckStateChanged(this, &SRelectionPluginMainWidget::OnUnboundCheckBoxStateChanged)
					.IsChecked(this, &SRelectionPluginMainWidget::IsUnboundCheckBoxChecked)
					]
			]

		// Adds capture sphere button
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.OnClicked(this, &SRelectionPluginMainWidget::OnAddsCaptureClickedEvent)
			.Text(FText::FromString("Adds reflection capture"))
			]
		
		// Caprure shape options
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Is sphere reflection capture"))
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Top)
				[
					// Needs to change to drop down menu
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &SRelectionPluginMainWidget::OnCaptureCheckBoxStateChanged)
					.IsChecked(this, &SRelectionPluginMainWidget::IsCaptureCheckBoxChecked)
				]
		]

		// Mesh title
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Mesh"))
			.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
			]
			]

		// Adds test meshes to the scene
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString("Adds test meshes"))
			.OnClicked(FOnClicked::CreateSP(this, &SRelectionPluginMainWidget::OnAddTestObjectClickedEvent))
			]

		// Determine whether mesh is static or movable.
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Movable?"))
			]

		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			[
				// Needs to change to drop down menu
				SNew(SCheckBox)
			]
			]

		// Performance title
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Optimization"))
			.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
			]
			]

		// Reflection quality control
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Reflection Quality"))
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Top)
				[

					SNew(SNumericEntryBox<int32>)
					.AllowSpin(true)
					.MinValue(0).MaxValue(100)
					.MinSliderValue(0).MaxSliderValue(100)
					.OnValueChanged(
						this, &SRelectionPluginMainWidget::SetReflectionQuality)
					.Value(mReflectionQualityControl)
				]
		]

		// Reflection map size
		+ SVerticalBox::Slot().Padding(topLevelPadding)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Reflection Map Size"))
				]

				+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top)
					[
						SNew(SNumericEntryBox<int16>)
						.AllowSpin(true)
					.MinValue(32).MaxValue(2048)
					.MinSliderValue(32).MaxSliderValue(2048)
					.OnValueChanged(this, &SRelectionPluginMainWidget::SetReflectionMapSize)
					.Value(mReflectionMapSizeControl)
					]
		]

	];
}

FReply SRelectionPluginMainWidget::OnInitializationClickedEvent()
{
	UE_LOG(LogTemp, Verbose, TEXT("Postprocessing volume added to the scene!"));
	AActor* foundActor = nullptr;
	foundActor = FindActor(APostProcessVolume::StaticClass());

	if (!foundActor) {
		UE_LOG(LogTemp, Warning, TEXT("Can't find the postprocessing volume! Create a new one!"));
		foundActor = AddActor(APostProcessVolume::StaticClass());
	}

	APostProcessVolume* volumePtr = Cast<APostProcessVolume>(foundActor);
	volumePtr->bUnbound = bPostprocessingUnbound;
	switch (mInitializationOption) {
	case InitializationOptions::LUMEN:
		volumePtr->Settings.bOverride_ReflectionMethod = true;
		volumePtr->Settings.ReflectionMethod = EReflectionMethod::Lumen;
		volumePtr->Settings.bOverride_DynamicGlobalIlluminationMethod = true;
		volumePtr->Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Lumen;
		volumePtr->Settings.bOverride_LumenReflectionQuality = true;
		volumePtr->Settings.bOverride_ScreenSpaceReflectionQuality = false;
		volumePtr->Settings.LumenReflectionQuality = GetLumenReflectionQuality();
		break;
	case InitializationOptions::SSRefection:
		volumePtr->Settings.bOverride_ReflectionMethod = true;
		volumePtr->Settings.ReflectionMethod = EReflectionMethod::ScreenSpace;
		volumePtr->Settings.bOverride_DynamicGlobalIlluminationMethod = true;
		volumePtr->Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::ScreenSpace;
		volumePtr->Settings.bOverride_ScreenSpaceReflectionQuality = true;
		volumePtr->Settings.bOverride_LumenReflectionQuality = false;
		volumePtr->Settings.ScreenSpaceReflectionQuality = GetSSReflectionQuality();
		break;
	case InitializationOptions::Capture:
		volumePtr->Settings.bOverride_ReflectionMethod = true;
		volumePtr->Settings.ReflectionMethod = EReflectionMethod::None;
		volumePtr->Settings.bOverride_ReflectionMethod = true;
		volumePtr->Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::ScreenSpace;
		volumePtr->Settings.bOverride_ScreenSpaceReflectionQuality = false;
		volumePtr->Settings.bOverride_LumenReflectionQuality = false;
		break;
	}

	return FReply::Handled();
}

void SRelectionPluginMainWidget::OnUnboundCheckBoxStateChanged(ECheckBoxState NewState)
{
	bPostprocessingUnbound = NewState == ECheckBoxState::Checked ? true : false;
}

ECheckBoxState SRelectionPluginMainWidget::IsUnboundCheckBoxChecked() const
{
	return bPostprocessingUnbound ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

FReply SRelectionPluginMainWidget::OnAddsCaptureClickedEvent()
{
	if (bSphereCapture) {
		AddActor(ASphereReflectionCapture::StaticClass());
	}
	else {
		AddActor(ABoxReflectionCapture::StaticClass());
	}
	
	return FReply::Handled();
}

void SRelectionPluginMainWidget::OnCaptureCheckBoxStateChanged(ECheckBoxState NewState)
{
	bSphereCapture = NewState == ECheckBoxState::Checked ? true : false;
}

ECheckBoxState SRelectionPluginMainWidget::IsCaptureCheckBoxChecked() const
{
	return bSphereCapture ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

FReply SRelectionPluginMainWidget::OnInitOptionLumenClickedEvent()
{
	if (IsMobilePlatform()) {
		OnInitOptionSSReflectionClickedEvent();
		UE_LOG(LogTemp, Warning, TEXT("Current running on mobile platform, default to SS reflection."));
		return FReply::Handled();
	}
	mInitializationOption = InitializationOptions::LUMEN;
	return FReply::Handled();
}

FReply SRelectionPluginMainWidget::OnInitOptionSSReflectionClickedEvent()
{
	mInitializationOption = InitializationOptions::SSRefection;
	return FReply::Handled();
}

FReply SRelectionPluginMainWidget::OnInitOptionCaptureClickedEvent()
{
	mInitializationOption = InitializationOptions::Capture;
	return FReply::Handled();
}

FText SRelectionPluginMainWidget::GetInitializationButtonTitle() const
{
	switch (mInitializationOption) {
	case InitializationOptions::Capture:
		return FText::FromString("Capture Only");
	case InitializationOptions::SSRefection:
		return FText::FromString("SSReflection + Capture");
	default:
		return FText::FromString("Lumen");
	}
	return FText();
}

float SRelectionPluginMainWidget::GetLumenReflectionQuality()
{
	float scaleFactor = 1.75f;
	float res = mReflectionQuality * scaleFactor / 100.0 + 0.25f;
	return res;
}

float SRelectionPluginMainWidget::GetSSReflectionQuality()
{
	return mReflectionQuality;
}

FReply SRelectionPluginMainWidget::OnAddTestObjectClickedEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("New test object added to the scene!"));

	return FReply::Handled();
}

void SRelectionPluginMainWidget::SetReflectionQuality(int32 val)
{
	
	mReflectionQuality = val;
}

TOptional<int32> SRelectionPluginMainWidget::GetReflectionQuality() const
{
	return mReflectionQuality;
}

void SRelectionPluginMainWidget::SetReflectionMapSize(int16 val)
{
	// Good!
	URendererSettings* Settings = GetMutableDefault<URendererSettings>();
	if (Settings) {
		Settings->ReflectionCaptureResolution = mReflectionMapSize;
		Settings->SaveConfig();
	}
	mReflectionMapSize = val;

}

TOptional<int16> SRelectionPluginMainWidget::GetReflectionMapSize() const
{
	return mReflectionMapSize;
}

AActor* SRelectionPluginMainWidget::FindActor(TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> foundActors;
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (world) {
		UGameplayStatics::GetAllActorsOfClass(world, ActorClass, foundActors);
		if (foundActors.Num() > 0) {
			return foundActors[0];
		}
	}
	return nullptr;
}

AActor* SRelectionPluginMainWidget::AddActor(TSubclassOf<AActor> ActorClass)
{
	
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (!world) {
		UE_LOG(LogTemp, Warning, TEXT("Can't find the world! Cannot spawn actors!"));
		return nullptr;
	}
	ULevel* level = world->GetCurrentLevel();
	if (!level) {
		UE_LOG(LogTemp, Warning, TEXT("Can't find the level! Cannot spawn actors!"));
		return nullptr;
	}
	
	return GEditor->AddActor(level, ActorClass, FTransform());
}

bool SRelectionPluginMainWidget::IsMobilePlatform() const
{
	// This is not correct for the release. We probably need a target platform check, given
	// the plugin is going to run on Desktop anyways.
	return mGamePlatformName.Equals("Android") || mGamePlatformName.Equals("IOS");
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
