from django.db import models
from django.urls import reverse

# Create your models here.

class Algorithm(models.Model):
    name = models.CharField(max_length = 40)
    author = models.CharField(max_length = 30)  # nick użytkownika serwisu
    alg_file = models.FileField()

    def get_absolute_url(self):
        return reverse('algorithms:detail', kwargs={'pk' : self.pk})

    def __str__(self):
        return self.name

# gdzie umieścić ścieżki do zdjęć do odpowiednich wykresów?

class Benchmark(models.Model):
    name = models.CharField(max_length = 40)

    def __str__(self):
        return self.name

class Outcome(models.Model):
    algorithm = models.ForeignKey(Algorithm, on_delete = models.CASCADE)
    benchmark = models.ForeignKey(Benchmark, on_delete = models.CASCADE)
    dimension = models.IntegerField()