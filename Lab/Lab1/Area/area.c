#include <stdio.h>

int main() {
    float radius, area;
    printf("Enter the radius: ");
    scanf("%f", &radius);    //user input
    area = 3.14159 * radius * radius;//calclution
    printf("Area of the circle = %.2f\n", area);//output
    return 0;
}