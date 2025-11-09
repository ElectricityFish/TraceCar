#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE );
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6 );//选择6分频（12MHz）
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  =GPIO_Mode_AIN ;//选择模拟输入模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//选择规则组的输入通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4 ,1,ADC_SampleTime_55Cycles5);//第三个参数是规则组序列器里的次序,第四个参数是采样时间
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode =DISABLE;//选择使用连续转换还是单次转换
	ADC_InitStruct.ADC_DataAlign =ADC_DataAlign_Right ;//数据对齐，一般右对齐
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None ; //外部触发转换选择，就是触发控制的触发源,这里使用软件触发
	ADC_InitStruct.ADC_Mode =ADC_Mode_Independent;//ADC模式（独立还是双ADC模式）
	ADC_InitStruct.ADC_NbrOfChannel=1; //指定在扫描模式下总共会用到几个通道
	ADC_InitStruct.ADC_ScanConvMode=DISABLE; //选择扫描模式还是非扫描模式
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	
	ADC_Cmd(ADC1,ENABLE);//开启ADC的电源
	
	
	//对ADC进行校准(固定步骤)
	ADC_ResetCalibration( ADC1);
	while(ADC_GetResetCalibrationStatus( ADC1)==SET);
	ADC_StartCalibration( ADC1);
	while(ADC_GetCalibrationStatus( ADC1)==SET);
	
	//完成
}

//获取转换结果
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发转换的函数(如果是连续转换模式这个函数可以放在Init里，也不需要判断转换是否完成)
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//获取标志位状态判断转换是否完成
	
	return ADC_GetConversionValue(ADC1);//获取转换结果,这个函数可以自动清除标志位
}

