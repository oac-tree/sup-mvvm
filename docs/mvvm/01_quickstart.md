# Quick start

```C++

class GaussianDistribution 
{
public:
  Circle() : xc(0.0), yc(0.0), radius(1.0) {}
  Circle(double xc, double yc, double radius) : xc(xc_), yc(yc_), radius(radius) {}

  double SetRadius(double radius) const {radius_ = radius}
  double GetRadius() const {return radius_}

private:
  double xc_;
  double yc_;
  double radius_;
}

```
