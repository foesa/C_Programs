package armcalc;

public class armcalc {
	public String buttonType(boolean button,String type){
		if(button == true);
		return "";
	}
	public static boolean longPress(boolean buttonPressed){
		if(buttonPressed) return true;
		return false;
	}
	public static void main(String[] args){
		boolean button = false;
		String type ="";
		boolean first = false;
		int num1 =0;
		int num2 =0;
		int operand =0;
		while(type.equals("exit")){
			if(longPress(button) && type == "+"){
				if(first == false){
					num2 = num1;
					num1 =0;
					first = true;
					//clear display
				}
				else{
					if(operand == 1){
						num2 = num1 + num2;
						//display this
					}
					else if(operand == 2){
						num2 = num2 -num1;
						//display this;
					}
				}
				operand = 0;
			}
			else if(longPress(button) && type == "-"){
				//same as + but operand = 2 
			}
			else if(longPress(button) && type == "add"){
				num1++;
			}
			else if(longPress(button) && type == "sub"){
				num1--;
			}
			else if(!longPress(button) && type == "+"){
				//not sure about waht needs to be done here
			}
			else if(!longPress(button) && type == "-"){
				num1 = 0;
				num2= 0;
				//clear leds
				first = false;
				operand = 0;
			}
		}
	}
}
