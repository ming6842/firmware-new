#include "stm32f4xx_conf.h"
#include "input_capture.h"
input_capture_t inc[] = {
	[INC1] = {
		.status = RISING,
		.curr_value = 0,
		.prev_value = 0
 
	},
	[INC2] = {
		.status = RISING,
		.curr_value = 0,
		.prev_value = 0
	},
	[INC3] = {
		.status = RISING,
		.curr_value = 0,
		.prev_value = 0
	},
	[INC4] = {
		.status = RISING,
		.curr_value = 0,
		.prev_value = 0
	},
	[INC5] = {
		.status = RISING,
		.curr_value = 0,
		.prev_value = 0
	},
	[INC6] = {
		.status = RISING,
		.curr_value = 0,
		.prev_value = 0
	}

};



void TIM2_IRQHandler(void)
{
	uint32_t current[4];
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) {
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		if ( inc[INC6].status == RISING ) {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;

			/* Get the Input Capture value */
			inc[INC6].prev_value = TIM_GetCapture1(TIM2);
			inc[INC6].status = FALLING;
			

		} else {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;

			//Get the Input Capture value
			current[0] =  TIM_GetCapture1(TIM2);

			if (current[0] > inc[INC6].prev_value)
				inc[INC6].curr_value =  current[0] - inc[INC6].prev_value ;
			else if (current[0] < inc[INC6].prev_value)
				inc[INC6].curr_value = 0xFFFF - inc[INC6].prev_value  + current[0] ;

			inc[INC6].status = RISING;
		}

		TIM_ICInit(TIM2, &TIM_ICInitStructure);

	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) {
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

		if (inc[INC5].status == RISING) {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;

			/* Get the Input Capture value */
			inc[INC5].prev_value = TIM_GetCapture2(TIM2);
			inc[INC5].status = FALLING;

		} else {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;

			/* Get the Input Capture value */
			current[1] =  TIM_GetCapture2(TIM2);

			if (current[1] > inc[INC5].prev_value)
				inc[INC5].curr_value =  current[1] - inc[INC5].prev_value ;
			else if (current[1] < inc[INC5].prev_value)
				inc[INC5].curr_value = 0xFFFF - inc[INC5].prev_value + current[1] ;

			inc[INC5].status = RISING;


		}

		TIM_ICInit(TIM2, &TIM_ICInitStructure);
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET) {
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);

		if (inc[INC2].status == RISING) {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;

			/* Get the Input Capture value */
			inc[INC2].prev_value = TIM_GetCapture3(TIM2);
			inc[INC2].status = FALLING;
			


		} else {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;

			/* Get the Input Capture value */
			current[2] =  TIM_GetCapture3(TIM2);

			if (current[2] > inc[INC2].prev_value)
				inc[INC2].curr_value=  current[2] - inc[INC2].prev_value;
			else if (current[2] < inc[INC2].prev_value)
				inc[INC2].curr_value =   0xFFFF - inc[INC2].prev_value + current[2] ;
			
			inc[INC2].status = RISING;


		}

		TIM_ICInit(TIM2, &TIM_ICInitStructure);
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET) {
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);

		if (inc[INC1].status == RISING) {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;

			/* Get the Input Capture value */
			inc[INC1].prev_value = TIM_GetCapture4(TIM2);
			inc[INC1].status  = FALLING;
			

		} else {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;

			/* Get the Input Capture value */
			current[3] =  TIM_GetCapture4(TIM2);

			if (current[3] > inc[INC1].prev_value)
				inc[INC1].curr_value =  current[3] - inc[INC1].prev_value;
			else if (current[3] < inc[INC1].prev_value)
				inc[INC1].curr_value = 0xFFFF - inc[INC1].prev_value+ current[3] ;

			inc[INC1].status = RISING;


		}

		TIM_ICInit(TIM2, &TIM_ICInitStructure);
	}
}

void TIM5_IRQHandler()
{

	uint32_t current[2];
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);

	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET) {
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);

		if (inc[INC4].status == RISING) {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;

			/* Get the Input Capture value */
			inc[INC4].prev_value = TIM_GetCapture1(TIM5);
			inc[INC4].status = FALLING;
			
		} else {

			TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;

			/* Get the Input Capture value */
			current[0] =  TIM_GetCapture1(TIM5);

			if (current[0] > inc[INC4].prev_value)
				inc[INC4].curr_value =  current[0] - inc[INC4].prev_value;
			else if (current[0] < inc[INC4].prev_value)
				inc[INC4].curr_value = 0xFFFF - inc[INC4].prev_value+ current[0] ;

			inc[INC4].status = RISING;

		}
		TIM_ICInit(TIM5, &TIM_ICInitStructure);
	}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET) {
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
		if (inc[INC3].status == RISING) {
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;

			/* Get the Input Capture value */
			inc[INC3].prev_value = TIM_GetCapture2(TIM5);
			inc[INC3].status = FALLING;
	
		} else {

			TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;

			/* Get the Input Capture value */
			current[1] =  TIM_GetCapture2(TIM5);

			if (current[1] > inc[INC3].prev_value)
				inc[INC3].curr_value =  current[1] - inc[INC3].prev_value;
			else if (current[1] < inc[INC3].prev_value)
				inc[INC3].curr_value = 0xFFFF - inc[INC3].prev_value+ current[1] ;

			inc[INC3].status = RISING;

		}
		TIM_ICInit(TIM5, &TIM_ICInitStructure);
	}

}