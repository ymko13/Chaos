#pragma once
class mPlayerCharacter : public mCharacter
{
public:
	mPlayerCharacter(void);
	~mPlayerCharacter(void);

	void InitializeProperties() HKV_OVERRIDE;
	void PostInitialize() HKV_OVERRIDE;

	void ThinkFunction() HKV_OVERRIDE;
	
	void AddControllerComponent(VType* controllerType) HKV_OVERRIDE;

protected:

private:
	V_DECLARE_SERIAL_DLLEXP(mPlayerCharacter, SAMPLEPLUGIN_IMPEXP);
	V_DECLARE_VARTABLE(mPlayerCharacter, SAMPLEPLUGIN_IMPEXP);
};

