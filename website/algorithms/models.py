from django.db import models

# Create your models here.

class Algorithm(models.Model):
    name = models.CharField(max_length = 40)
    author = models.CharField(max_length = 30)  # nick użytkownika serwisu

    def __str__(self):
        return self.name

class Benchmark(models.Model):
    name = models.CharField(max_length = 40)

    def __str__(self):
        return self.name

class Outcome(models.Model):
    algorithm = models.ForeignKey(Algorithm, on_delete = models.CASCADE)
    benchmark = models.ForeignKey(Benchmark, on_delete = models.CASCADE)
    dimension = models.IntegerField()